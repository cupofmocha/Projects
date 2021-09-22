#/usr/bin/python3
import numpy as np
import hashlib
from random import seed
from random import randint
import time

p = 168199388701209853920129085113302407023173962717160229197318545484823101018386724351964316301278642143567435810448472465887143222934545154943005714265124445244247988777471773193847131514083030740407543233616696550197643519458134465700691569680905568000063025830089599260400096259430726498683087138415465107499

q = 959452661475451209325433595634941112150003865821

g = 94389192776327398589845326980349814526433869093412782345430946059206568804005181600855825906142967271872548375877738949875812540433223444968461350789461385043775029963900638123183435133537262152973355498432995364505138912569755859623649866375135353179362670798771770711847430626954864269888988371113567502852

pk1 = 49336018324808093534733548840411752485726058527829630668967480568854756416567496216294919051910148686186622706869702321664465094703247368646506821015290302480990450130280616929226917246255147063292301724297680683401258636182185599124131170077548450754294083728885075516985144944984920010138492897272069257160

sk1 = 432398415306986194693973996870836079581453988813
sk2 = 165849943586922055423650237226339279137759546603
sk3 = 627658512551971075308886219669315148725310346887
amnt0 = 1
amnt1 = 2
amnt2 = 3
amnt3 = 4

# Question 1
# x is some sk, n is p
def square_and_multiply_and_modulo(g, x, n):
    result = 1
    # First convert x to a binary string
    x_binary = str(format(x, "b"))
    for i in x_binary:
        result = result * result % n
        if i == "1":
            result = (result * g) % n
    return result

# Verify (sk1, pk1)
#if (square_and_multiply_and_modulo(g, sk1, p) == pk1):
    #print("(sk1, pk1) is verified.")
#else:
    #print("(sk1, pk1) is NOT verified.")

# Find pk2 for (sk2, pk2):
pk2 = square_and_multiply_and_modulo(g, sk2, p)
#print(pk2)

# Find pk3 for (sk3, pk3):
pk3 = square_and_multiply_and_modulo(g, sk3, p)
#print(pk3)



# Question 2
def sha224(value):
    hexMessage = hex(value)
    hexMessage = hexMessage[2:]
    m_bytearray = bytes(hexMessage, encoding='utf-8')
    hexMessage = hashlib.sha3_224(m_bytearray).hexdigest()
    int_value = int(hexMessage, base=16)
    return int_value



# Question 3 to generate message
def generate_message(pki, pkj, amntk):
    # First convert to binary strings
    pki_binary = str(format(pki, "b"))
    pkj_binary = str(format(pkj, "b"))
    amntk_binary = str(format(amntk, "b"))
    # Then slice them according to the requirements
    pki_399 = pki_binary[0:399]
    pkj_399 = pkj_binary[0:399]
    # Concatenate to the desired message format
    temp = pki_399 + pkj_399 + amntk_binary
    # Finally convert to integer representation to get the message
    message = int(temp, 2)
    #print(message)
    return message
    
m1 = generate_message(pk1, pk2, amnt1)
m2 = generate_message(pk2, pk3, amnt2)



# Question 4
# Signature Generation
def generate_signature(m, p, q, g, ski):
    # Generate k by PRNG
    seed(1)
    k = randint(1, q-1)
    r = square_and_multiply_and_modulo(g, k, p)
    hash_m = sha224(m)
    mod_inverse = pow(k, -1, q)
    s = (((sha224(m) - (ski * r)) % q) * mod_inverse) % q # By the multiplicative property of modulo inverse
    return r, s

sig1 = generate_signature(m1, p, q, g, sk1)
sig2 = generate_signature(m2, p, q, g, sk2)

# Signature Verification
def verify_signature(mi, ri, si, g, p, q, pki):
    if (ri < 0 or ri > p or si < 0 or si > p):
        #print("Signature is not verified.")
        return
    s_mod_inverse = pow(si, -1, q)
    hash_m = sha224(mi)
    u = (hash_m % q * s_mod_inverse) % q
    #print("u is", u)
    v = ((-ri % q * s_mod_inverse) % q)
    #print("v is", v)
    w = (square_and_multiply_and_modulo(g, u, p) * square_and_multiply_and_modulo(pki, v, p)) % p # By the multiplicative property of modulo inverse
    #print("w is", w)
    #if (w == ri):
        #print("Signature is successfully verified.")
    #else:
        #print("Signature is NOT verified.")

r1 = sig1[0]
#print("r is", r1)
s1 = sig1[1]
#print("s is", s1)

verify_signature(m1, r1, s1, g, p, q, pk1)

# Question 5
def find_nonce(amti_1, amti_2, ski_1, ski_2):
    # Start time of the function
    start_time = time.time()

    # Hash of amnt0
    hash1 = sha224(amti_1)

    # Get the message
    pk1 = square_and_multiply_and_modulo(g, ski_1, p)
    pk2 = square_and_multiply_and_modulo(g, ski_2, p)
    m1 = generate_message(pk1, pk2, amti_2)

    # Convert hash1 and m1 to binary
    hash1_binary = str(format(hash1, "b"))
    m1_binary = str(format(m1, "b"))

    # Initial nonce. Start at 0 and keep adding
    nonce = ""
    for i in range(124):
        nonce += "0"

    # 24 length string of 0
    zero_string = ""
    for i in range(24):
        zero_string = zero_string + "0"


    t_value = sha224(int(hash1_binary + m1_binary + nonce, 2))
    t_value = str(format(t_value, "b"))

    while (t_value[:24] != zero_string): 
        nonce = int(nonce, 2)
        nonce += 1

        nonce = str(format(nonce, "b"))
        while len(nonce) != 124:
            nonce = "0" + nonce

        #print(nonce1)

        t_value = sha224(int(hash1_binary + m1_binary + nonce, 2))
        t_value = str(format(t_value, "b"))

        while (len(t_value) != 224):
            t_value = "0" + t_value

        #print(t_value[:24])
    end_time = time.time()
    #print(end_time - start_time)
    return t_value

def verify_nonce(amti_1, amti_2, ski_1, ski_2, nonce):
    # Hash of amnt0
    hash1 = sha224(amti_1)

    # Get the message
    pk1 = square_and_multiply_and_modulo(g, ski_1, p)
    pk2 = square_and_multiply_and_modulo(g, ski_2, p)
    m1 = generate_message(pk1, pk2, amti_2)

    # Convert hash1 and m1 to binary
    hash1_binary = str(format(hash1, "b"))
    m1_binary = str(format(m1, "b"))

    zero_string = ""
    for i in range(24):
        zero_string = zero_string + "0"

    nonce_hash = sha224(int(hash1_binary + m1_binary + nonce, 2))

    #if nonce_hash[:24] == zero_string:
        #print("Verified")


    #print(nonce1)
