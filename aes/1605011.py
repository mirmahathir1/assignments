from AES import *

plain_text = "Two One Nine Two"
plain_text = plain_text.ljust(16,chr(0))[:16]

key = "Thats my Kung Fu"
key = key.ljust(16,chr(0))[:16]

plain_text_bitvector = BitVector(textstring = plain_text)
key_bitvector = BitVector(textstring = key)

# print("PLAINTEXT: ",plain_text_bitvector.get_hex_string_from_bitvector())
# print("KEY: ",key_bitvector.get_hex_string_from_bitvector())

round_keys = generate_all_round_keys(key_bitvector)

# ROUND 0
# print("ROUND", 0, "start", "-" * 40)

state_matrix_round0 = plain_text_bitvector.__xor__(round_keys[0])

# print("result: ",state_matrix_round0.get_hex_string_from_bitvector())

state_matrix_round = state_matrix_round0

# ROUND 1-9
for round in range(1,10):
    # print("ROUND",round,"start", "-" * 40)
    # SUBSTITUTE
    state_matrix_round = substitute(state_matrix_round)
    # print("after substitute:")
    # print(state_matrix_round.get_hex_string_from_bitvector())

    state_matrix_transformed = matrix1Dto2D(state_matrix_round)

    # SHIFT ROWS
    for i in range(1,4):
        state_matrix_transformed[i] = state_matrix_transformed[i][i:] + state_matrix_transformed[i][:i]

    # print("after shift row: ")
    # print_matrix(state_matrix_transformed)

    # MIX COLUMN
    multiplied_matrix = matrix_multiplication(Mixer,state_matrix_transformed)
    # print("after mix column: ")
    # print_matrix(multiplied_matrix)

    multiplied_matrix_lineared = matrix2Dto1D(multiplied_matrix)

    # ADD ROUND KEY
    state_matrix_round = BitVector(hexstring=multiplied_matrix_lineared).__xor__(round_keys[round])
    # print("after adding round key ",state_matrix_round.get_hex_string_from_bitvector(),": ")
    # print("result:",state_matrix_round.get_hex_string_from_bitvector())


# ROUND 10
# print("ROUND 10 start","-"*40)
# print("state matrix: ")
# print(state_matrix_round.get_hex_string_from_bitvector())

# SUBSTITUTE
state_matrix_round = substitute(state_matrix_round)
# print("after substitute: ")

# print(state_matrix_round.get_hex_string_from_bitvector())

state_matrix_transformed = matrix1Dto2D(state_matrix_round)

# SHIFT ROWs
for i in range(1,4):
    state_matrix_transformed[i] = state_matrix_transformed[i][i:] + state_matrix_transformed[i][:i]

multiplied_matrix_lineared = matrix2Dto1D(state_matrix_transformed)

# print("after shifting row: ")
# print(BitVector(hexstring=multiplied_matrix_lineared).get_hex_string_from_bitvector())

# ADD ROUND KEY
state_matrix_round = BitVector(hexstring=multiplied_matrix_lineared).__xor__(round_keys[10])

print("result:",state_matrix_round.get_hex_string_from_bitvector())


