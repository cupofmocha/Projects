#/usr/bin/python3
import numpy as np
# import matplotlib.pyplot as plt

# The 26 alphabets
alphabet = ("a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z")

# Excercise 1 Part 1
# Perform frequency analysis by counting the number of occurence of alphabets
def frequency_analysis(textFile):

    # Initialize array for frequency analysis
    freqArray = np.zeros(26)

    # Open the ciphertext from the file
    file = open(textFile, "r")
    content = file.read()

    # Goes through each letter in the ciphertext to assign them to the frequency analsysis array
    for i in range (len(content)):
        for j in range(26):
            if content[i] == alphabet[j]:
                freqArray[j] = freqArray[j] + 1
    
    
    print(freqArray)
    # plt.plot(alphabet, freqArray)
    # plt.xlabel('Alphabets')
    # plt.ylabel('Frequency')
    # plt.show()


# Excercise 1 Part 2
# Perform frequency analysis by counting the number of occurence of alphabets in chunks
def frequency_analysis_chunk(textFile, keyLen):

    # Initialize 2D array for frequency analysis
    freqArray = np.zeros((keyLen, 26))

    # Open the ciphertext from the file
    file = open(textFile, "r")
    content = file.read()

    # Goes through each letter in the ciphertext to assign them to the frequency analsysis array
    for i in range (len(content)):
        for j in range(26):
            if content[i] == alphabet[j]:
                freqArray[ i % keyLen, j] += 1


    print(freqArray)
    # plt.plot(alphabet, freqArray[0])
    # plt.xlabel('Alphabets')
    # plt.ylabel('Frequency')
    # plt.show()

    return freqArray

# Excercise 1 Part 3
# Takes the frequency array from the previous function and obtain the key through frequency analysis assuming "e" is the most frequent letter
def decryptKey(freqArray):
    
    # Initalize key string
    key = ""

    # Loops through each chunk 
    for i in range(len(freqArray)):
        # Obtain the alphabet with the highest frequency
        maxFreqIndex = np.argmax(freqArray[i])
        # Obtain key value assuming "e" has the highest occurence
        key += alphabet[(maxFreqIndex + 26 - 4) % 26 ]

    print(key)
    return key


# Excercise 1 Part 4
# Using the decryptKey function and frequency_analysis_chunk function, decrypt the ciphertext
# Textfile and length of key as input
def decryptCiphertext(textFile, keyLen):  

    # Apply frequency analysis
    freqArray = frequency_analysis_chunk(textFile, keyLen)
    # Obtain the key
    key = decryptKey(freqArray)
    
    # Convert the letter keys into integers
    keyToNum = np.zeros(keyLen)
    for i in range(keyLen):
        for j in range(len(alphabet)):
            if (alphabet[j] == key[i]):
                keyToNum[i] = j
                break

    # Open the ciphertext from the file
    file = open(textFile, "r")
    content = file.read()

    # Using the key, decrypt the ciphertext
    plainText = ""
    for i in range(len(content)):
        for j in range(len(alphabet)):
            if (alphabet[j] == content[i]):
                decryptLetter = (j - keyToNum[i%keyLen] + 26) % 26
                plainText = plainText + alphabet[int(decryptLetter)]

    print(plainText)


# frequency_analysis("Ciphertext.txt")
# frequency_analysis("Ciphertext.txt", 6)
# decryptCiphertext("Ciphertext.txt", 6)







                
        