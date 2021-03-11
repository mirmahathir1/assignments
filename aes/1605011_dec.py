from AES import *

cypher_text = "29c3505f571420f6402299b31a02d73a"

key = "Thats my Kung Fu"
key = key.ljust(16,chr(0))[:16]

cypher_text_bitvector = BitVector(hexstring = cypher_text)
key_bitvector = BitVector(textstring = key)

#print("CYPHER: ",cypher_text_bitvector.get_hex_string_from_bitvector())
#print("KEY: ",key_bitvector.get_hex_string_from_bitvector())

round_keys = generate_all_round_keys(key_bitvector)

# ROUND 0
#print("ROUND 0 START","-"*40)
#print("after adding round key ",round_keys[10].get_hex_string_from_bitvector(),": ")
state_matrix_round0 = cypher_text_bitvector.__xor__(round_keys[10])

#print("RESULT: ",state_matrix_round0.get_hex_string_from_bitvector())

state_matrix_round = state_matrix_round0

for round in range(1,10):
    #print("ROUND",round,"START","-"*40)

    state_matrix_transformed = matrix1Dto2D(state_matrix_round)

    #print_matrix(state_matrix_transformed)
    # INVERSE SHIFT ROWS
    for i in range(1, 4):
        state_matrix_transformed[i] = state_matrix_transformed[i][-i:] + state_matrix_transformed[i][:-i]

    #print("after inverse shift")
    #print_matrix(state_matrix_transformed)

    # INVERSE SUBSTITUTE
    state_matrix_transformed = inverse_substitute(state_matrix_transformed)
    # print("after inverse substitute: ")
    # print_matrix(state_matrix_transformed)

    state_matrix_transformed_lineared = matrix2Dto1D(state_matrix_transformed)
    # print("after linearing: ",BitVector(hexstring=state_matrix_transformed_lineared).get_hex_string_from_bitvector())

    # ADD ROUND KEY
    round_key_added = BitVector(hexstring=state_matrix_transformed_lineared).__xor__(round_keys[10-round])

    # print("after adding round key ",round_keys[10-round].get_hex_string_from_bitvector(),": ")
    # print(round_key_added.get_hex_string_from_bitvector())

    # INVERSE MIX COLUMN
    round_key_added = matrix1Dto2D(round_key_added)
    multiplied = matrix_multiplication(InvMixer,round_key_added)
    # print("after inverse mix column: ")
    # print_matrix(multiplied)

    state_matrix_round = BitVector(hexstring=matrix2Dto1D(multiplied))
    # print("RESULT:",state_matrix_round.get_hex_string_from_bitvector())

# ROUND 10
# print("ROUND",10,"START","-"*40)

state_matrix_transformed = matrix1Dto2D(state_matrix_round)

# print_matrix(state_matrix_transformed)

# INVERSE SHIFT ROWS
for i in range(1, 4):
    state_matrix_transformed[i] = state_matrix_transformed[i][-i:] + state_matrix_transformed[i][:-i]

# print("after inverse shift row: ")
# print_matrix(state_matrix_transformed)

# INVERSE SUBSTITUTE
state_matrix_transformed = inverse_substitute(state_matrix_transformed)
# print("after inverse substitution: ")
# print_matrix(state_matrix_transformed)

state_matrix_transformed_lineared = matrix2Dto1D(state_matrix_transformed)

# ADD ROUND KEY
round_key_added = BitVector(hexstring=state_matrix_transformed_lineared).__xor__(round_keys[0])
# print("after adding round key ",round_keys[0].get_hex_string_from_bitvector(),":")

# print("RESULT: ",round_key_added.get_hex_string_from_bitvector())

def frombits(bits):
    chars = []
    for b in range(int(len(bits) / 8)):
        byte = bits[b*8:(b+1)*8]
        chars.append(chr(int(''.join([str(bit) for bit in byte]), 2)))
    return ''.join(chars)

plain_text = frombits(round_key_added)

print(plain_text)