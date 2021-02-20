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

#Initialize empty lists
names = []
qualifier_list = []

#Create connection to DB cluster
cluster = MongoClient(MONGO_URL)
db = cluster["tournament"]
collection = db["participant"]

#Create client
client = commands.Bot(command_prefix="~")

#functions
def qualifier_decider(num_players, num_qualifiers):
    query_list = collection.distinct("total_score") # find total score list (default ascending order)
    print(query_list)
    for x in range(0, num_qualifiers):
        total_score = query_list[num_players - 1 - x] # Couldn't get sort() working so iterate array from back to front to get descending order
        name_query = collection.distinct("name", {"total_score": total_score})
        qualifierName = ' '.join(name_query)
        #print(total_score)
        qualifier_list.append(qualifierName)
        #print(qualifierName)
    
    return ' '.join(qualifier_list)

@client.event
async def on_ready():
    print(f'{client.user} has connected to Discord!')

@client.command()
async def start(ctx):
    await ctx.send("Please enter number of participants: ")
    while True:
        try:
            input_numPlayers = await client.wait_for("message")
            input_numPlayers = int(input_numPlayers.content)
            break
        except:
            await ctx.send("Please enter an integer number.")  
    numPlayers = input_numPlayers
    #await ctx.send(numPlayers)

    await ctx.send("Please enter names of all participants, each separated by a space:")
    input_names = await client.wait_for("message")
    names = input_names.content.split(" ")

    for a in names:
        await ctx.send(a)

    #Post participants to db
    for i in range(0, numPlayers):
        post = {"_id": i, 
                "name": names[i], 
                "song1": "", 
                "song2": "", 
                "song3": "", 
                "song4": "", 
                "song5": "",
                "song1_score": 0,
                "song2_score": 0,
                "song3_score": 0,
                "song4_score": 0,
                "song5_score": 0,                 
                "total_score": 0}
        collection.insert_one(post)
        await ctx.send('accepted!')

    #Begin qualifier round
    await ctx.send("Qualifier round begins, enter number of players that will be qualified.")
    while True:
        try:
            input_numQualifiers = await client.wait_for("message")
            input_numQualifiers = int(input_numQualifiers.content)
            break
        except:
            await ctx.send("Please enter an integer number.")  
    numQualifiers = input_numQualifiers
    await ctx.send(numQualifiers)

    #Assuming 2 song selections are complete
    #Qualifier Round:
    for j in range(0, numPlayers):
        query = collection.distinct("name", {"_id": j})
        playerName = ' '.join(query) #Convert list to string using join() and list comprehension
        await ctx.send(playerName + " please enter score for song 1")
        while True:
            try:
                input_song1_score = await client.wait_for("message")
                input_song1_score = int(input_song1_score.content)
                break
            except:
                await ctx.send("Please enter an integer number.")  
        song1_score = input_song1_score

        collection.update_one(
            {"_id": j},
            {
                '$set': {"song1_score": song1_score}
            } 
        )
        # $project stage with $merge to sum up total_score and update its value with corresponding _id
        pipe = [{
            '$project':  
                {"_id": j,
                'total_score': {'$sum': {'$sum': ["$song1_score", "$song2_score", "$song3_score", "$song4_score", "$song5_score"]}}}},
                { '$merge' : { 'into': { 'db': "tournament", 'coll': "participant" }, 'on': "_id", 'whenNotMatched': "discard"}
        }]
        collection.aggregate(pipe)
    qualifiers = qualifier_decider(numPlayers, numQualifiers)
    await ctx.send("The following players are qualified for quarter-finals: " + qualifiers)

    #Quarter-finals
    # for j in range(0, numPlayers):
    #     query = collection.distinct("name", {"_id": j})
    #     playerName = ' '.join(query) #Convert list to string using join() and list comprehension
    #     await ctx.send(playerName + " please enter score for song 1")
    #     input_song1 = await client.wait_for("message")
    #     song1 = ' '.join(input_song1.content.split(" "))
    #     collection.update_one(
    #         {"_id": j},
    #         {
    #             '$set': {"song1": song1}
    #         } 
    #     )


@client.command()
async def exit(ctx):
    collection.delete_many({})
    if (collection.estimated_document_count() == 0):
        await ctx.send("Database cleared, bye!")


#keep_alive.keep_alive()
client.run(TOKEN)