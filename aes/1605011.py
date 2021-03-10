from BitVector import *
import numpy as np
# plain_text = input()
plain_text = "Two One Nine Two"
plain_text = plain_text.ljust(16,chr(0))[:16]

# key = input()
key = "Thats my Kung Fu"
key = key.ljust(16,chr(0))[:16]

plain_text_bitvector = BitVector(textstring = plain_text)

key_bitvector = BitVector(textstring = key)

Sbox = (
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16,
)

round_constant=BitVector(hexstring='01')


def w(word,index):
    return word[index * 8 * 4:(index + 1) * 8 * 4]

def g(word):
    left_shifted_word = word.__lshift__(8)

    left_shifted_word[0:8]=BitVector(intVal=Sbox[left_shifted_word[0:8].intValue()],size=8).__xor__(round_constant)
    left_shifted_word[8:16] = BitVector(intVal=Sbox[left_shifted_word[8:16].intValue()], size=8)
    left_shifted_word[16:24] = BitVector(intVal=Sbox[left_shifted_word[16:24].intValue()], size=8)
    left_shifted_word[24:32] = BitVector(intVal=Sbox[left_shifted_word[24:32].intValue()], size=8)
    return left_shifted_word


def round_key_generator(key):
    w4 = w(key,0).__xor__(g(w(key,3)))
    w5 = w4.__xor__(w(key,1))
    w6 = w5.__xor__(w(key,2))
    w7 = w6.__xor__(w(key,3))

    global round_constant
    round_constant = round_constant.gf_multiply_modular(BitVector(hexstring="02"), BitVector(bitstring='100011011'), 8)

    return BitVector(hexstring=w4.get_hex_string_from_bitvector()+w5.get_hex_string_from_bitvector()+w6.get_hex_string_from_bitvector()+w7.get_hex_string_from_bitvector())

def matrix_multiplication(M, N):
    R = [[BitVector(intVal=0,size=8) for j in range(4)] for i in range(4)]

    for i in range(0, 4):
        for j in range(0, 4):
            for k in range(0, 4):
                R[i][j] = R[i][j].__xor__(M[i][k].gf_multiply_modular(N[k][j], BitVector(bitstring='100011011'), 8))
    return R

round_keys=[]

round_keys.append(key_bitvector)

for i in range(1,11):
    round_keys.append(round_key_generator(round_keys[i-1]))

# for round_key in round_keys:
#     print(round_key.get_hex_string_from_bitvector())


# round 0
state_matrix_round0 = plain_text_bitvector.__xor__(round_keys[0])

# print(state_matrix_round0.get_hex_string_from_bitvector())

for i in range(16):
    state_matrix_round0[i*8:(i+1)*8]= BitVector(intVal=Sbox[state_matrix_round0[i*8:(i+1)*8].intValue()], size=8)

print("Rount 0 result: ",state_matrix_round0.get_hex_string_from_bitvector())

chunks, chunk_size = len(state_matrix_round0), int(len(state_matrix_round0)/16)
# print(chunk_size)
state_matrix0 = [state_matrix_round0[i:i+chunk_size] for i in range(0, chunks, chunk_size)]

state_matrix_transformed = [[],[],[],[]]

for i in range(16):
    state_matrix_transformed[i%4].append(state_matrix0[i])

for i in range(1,4):
    state_matrix_transformed[i] = state_matrix_transformed[i][i:] + state_matrix_transformed[i][:i]



# for row in range(4):
#     for col in range(4):
#         print(state_matrix_transformed[row][col].get_hex_string_from_bitvector()+" ",end='')
#     print("")


mix_column_matrix=[[],[],[],[]]
for i in range(16):
    mix_column_matrix[i%4].append(BitVector(intVal=1,size=8))

for i in range(4):
    mix_column_matrix[i][i]= BitVector(intVal=2,size=8)

mix_column_matrix[0][1] = BitVector(intVal=3,size=8)
mix_column_matrix[1][2] = BitVector(intVal=3,size=8)
mix_column_matrix[2][3] = BitVector(intVal=3,size=8)
mix_column_matrix[3][0] = BitVector(intVal=3,size=8)

# for row in range(4):
#     for col in range(4):
#         print(mix_column_matrix[row][col].get_hex_string_from_bitvector()+" ",end='')
#     print("")




multiplied_matrix = matrix_multiplication(mix_column_matrix,state_matrix_transformed)

# for row in range(4):
#     for col in range(4):
#         print(multiplied_matrix[row][col].get_hex_string_from_bitvector()+" ",end='')
#     print("")

print("-"*40)

multiplied_matrix_lineared = ""

for col in range(4):
    for row in range(4):
        multiplied_matrix_lineared+=multiplied_matrix[row][col].get_hex_string_from_bitvector()


cypher_text = BitVector(hexstring=multiplied_matrix_lineared).__xor__(round_keys[1])

print("Round 1 result: ",cypher_text.get_hex_string_from_bitvector())






