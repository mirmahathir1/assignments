from AES import *
file_name = "sample.txt"
in_file = open("encoded"+file_name, "rb")
out_file = open("decoded"+file_name, "wb")

round_keys = generate_all_round_keys("Thats my Kung Fu")

while True:
    data = in_file.read(32)
    if len(data) == 0:
        break

    decrypted_bit_vector = decrypt(data,round_keys)

    decrypted_bit_vector.write_to_file(out_file)

in_file.close()
out_file.close()