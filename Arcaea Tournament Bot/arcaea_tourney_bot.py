import asyncio
import discord
from discord.ext import commands
import pymongo
from pymongo import MongoClient
import urllib.parse
from discord.ext import commands
import os
from dotenv import load_dotenv
import keep_alive

load_dotenv()
TOKEN = os.getenv("BOT_TOKEN")
MONGO_URL = os.getenv("URL")

# List of Status
# DQ (Disqualified or eliminated)
# QF (In Quarter-finals)
# SF (In Semi-finals)
# F (In Finals)
# W (Winner)

#List of Channels
main_channel_id = 811318732097978428
lobby1_channel_id = 815712666592477214
lobby2_channel_id = 815712698746404866

# Initialize empty lists
names = []
qualifier_list = []

# Create connection to DB cluster
cluster = MongoClient(MONGO_URL)
db = cluster["tournament"]
collection = db["participant"]

# Create client
client = commands.Bot(command_prefix="~")

# Custom exception classes
class Error(Exception):
    """Base class for other exceptions"""
    pass
class ValueIsNegativeError(Error):
    """Raised when the number is strictly negative"""
    pass
class ValueIsNegativeOrZeroError(Error):
    """Raised when the number is negative or equal to 0"""
    pass
class NumQualifiersTooBigError(Error):
    """Raised when the number of qualifiers exceeds total players"""
    pass

# Functions
def find_name_by_id(player_id):
    query = collection.distinct("name", {"_id": player_id})
    name = ' '.join(query) #Convert list to string using join() and list comprehension
    return name

def qualifier_decider(num_players, num_qualifiers):
    query_list = collection.distinct("total_score") # find total score list (default ascending order)
    #print(query_list)
    for x in range(0, num_qualifiers):
        total_score = query_list[num_players - 1 - x] # Couldn't get sort() working so iterate array from back to front to get descending order
        name_query = collection.distinct("name", {"total_score": total_score})
        qualifier_name = ' '.join(name_query)
        #print(total_score)
        qualifier_list.append(qualifier_name)
        #print(qualifierName)
    
    return ' '.join(qualifier_list)

@client.event
async def on_ready():
    print(f'{client.user} has connected to Discord!')

@client.command()
async def start(ctx):
    main_channel = client.get_channel(main_channel_id)
    def check(m):
        return m.channel == ctx.channel # Checks and allows user messages Only from this channel
    if main_channel == ctx.channel: # Checks and allows user commands Only from this channel
        await ctx.channel.send("Please enter number of participants: ")
        while True:
            try:
                input_numPlayers = await client.wait_for("message", check=check)
                input_numPlayers = int(input_numPlayers.content)
                if input_numPlayers <= 0:
                    raise ValueIsNegativeOrZeroError
                break
            except ValueError:
                await ctx.channel.send("Please enter an integer number.")
            except ValueIsNegativeOrZeroError:
                await ctx.channel.send("Please enter a positive integer number.")      
        numPlayers = input_numPlayers
        #await ctx.channel.send(numPlayers)

        await ctx.channel.send("Please enter names of all participants, each separated by a space:")
        input_names = await client.wait_for("message", check=check)
        names = input_names.content.split(" ")

        for a in names:
            await ctx.channel.send(a)

        # Post participants to db
        for i in range(0, numPlayers):
            post = {"_id": i, 
                    "name": names[i], 
                    "song1": "", 
                    "song2": "", 
                    "song3": "", 
                    "song4": "", 
                    "song5": "",
                    "song1_score": "",
                    "song2_score": "",
                    "song3_score": "",
                    "song4_score": "",
                    "song5_score": "",                 
                    "total_score": "",
                    "status": ""}
            collection.insert_one(post)
            await ctx.channel.send('accepted!')

        # Begin qualifier round
        await ctx.channel.send("Qualifier round begins, enter number of players that will be qualified.")
        while True:
            try:
                input_numQualifiers = await client.wait_for("message", check=check)
                input_numQualifiers = int(input_numQualifiers.content)
                if input_numQualifiers <= 0:
                    raise ValueIsNegativeOrZeroError
                elif input_numQualifiers > numPlayers:
                    raise NumQualifiersTooBigError
                break
            except ValueError:
                await ctx.channel.send("Please enter an integer number.")
            except ValueIsNegativeOrZeroError:
                await ctx.channel.send("Please enter a positive integer number.")    
            except NumQualifiersTooBigError:
                await ctx.channel.send("Number of qualifiers cannot be greater than total number of players!")    
        numQualifiers = input_numQualifiers
        await ctx.channel.send(numQualifiers)

        # Assuming 2 song selections are complete
        # Qualifier Round:
        for j in range(0, numPlayers):
            player_name = find_name_by_id(j)
            await ctx.channel.send(player_name + " please enter score for song 1")
            while True:
                try:
                    input_song1_score = await client.wait_for("message", check=check)
                    input_song1_score = int(input_song1_score.content)
                    if input_song1_score < 0:
                        raise ValueIsNegativeError
                    break
                except ValueError:
                    await ctx.channel.send("Please enter an integer number.")
                except ValueIsNegativeError:
                    await ctx.channel.send("Please enter a positive integer number.")    
            song1_score = input_song1_score

            collection.update_one(
                {"_id": j},
                {
                    '$set': {"song1_score": song1_score}
                } 
            )
            # Need to also ask players for song2 score!

            # $project stage with $merge to sum up total_score and update its value with corresponding _id
            pipe = [{
                '$project':  
                    {"_id": j,
                    'total_score': {'$sum': {'$sum': ["$song1_score", "$song2_score", "$song3_score", "$song4_score", "$song5_score"]}}}},
                    { '$merge' : { 'into': { 'db': "tournament", 'coll': "participant" }, 'on': "_id", 'whenNotMatched': "discard"}
            }]
            collection.aggregate(pipe)
            
        qualifiers = qualifier_decider(numPlayers, numQualifiers)
        # Update status of all participants
        for k in range (0, numPlayers):
            player_name = find_name_by_id(k)
            if player_name in qualifiers:
                collection.update_one(
                    {"name": player_name},
                    {
                        '$set': {"status": "QF"}
                    }
                )
            else:
                collection.update_one(
                    {"name": player_name},
                    {
                        '$set': {"status": "DQ"}
                    }
                )
    
        await ctx.channel.send("The following players are qualified for quarter-finals: " + qualifiers)
        await asyncio.sleep(3)
        # random assign QF players to each lobby or manually assign via user input?
        await ctx.channel.send("Please enter mode of match-making for QF: enter \"rand\" for random selection, or \"manual\" for manual selection")

        # Quarter-finals
        # for j in range(0, numPlayers):
        #   playerName = find_name_by_id(j)
        #   await ctx.channel.send(playerName + " please enter score for song 1")
        #   input_song1 = await client.wait_for("message", check=check)
        #   song1 = ' '.join(input_song1.content.split(" "))
        #   collection.update_one(
        #       {"_id": j},
        #       {
        #             '$set': {"song1": song1}
        #       } 
        #   )
# @client.command()
# async def qf_lobby1(ctx):
#     lobby1_channel = client.get_channel(lobby1_channel_id)
#     def check(m):
#         return m.channel == ctx.channel
#     if lobby1_channel == ctx.channel:


@client.command()
async def exit(ctx):
    collection.delete_many({})
    if (collection.estimated_document_count() == 0):
        await ctx.channel.send("Database cleared, bye!")


#keep_alive.keep_alive()
client.run(TOKEN)