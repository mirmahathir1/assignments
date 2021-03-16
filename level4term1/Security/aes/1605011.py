from AES import *
import time
plain_text = "Two One Nine Two"
print("Plain text: ",plain_text)

key = "Thats my Kung Fu"
print("Key: ",key)

round_keys = generate_all_round_keys(key)

encryption_start_time = time.time()
cypher_text = encrypt(plain_text.encode(),round_keys)

# plain_text
print("Encryption Time: %s seconds" % (time.time() - encryption_start_time))
print("Cypher text: ",cypher_text)

decryption_start_time = time.time()
plain_text_new = decrypt(cypher_text.encode(),round_keys)
print("Decryption Time: %s seconds" % (time.time() - decryption_start_time))

plain_text_stringify = frombits(plain_text_new)

print("Decrypted Text: ",plain_text_stringify)
