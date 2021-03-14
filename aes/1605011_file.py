from AES import *
file_name = "sample.txt"
in_file = open(file_name, "rb")
out_file = open("encoded"+file_name, "wb")

round_keys = generate_all_round_keys("Thats my Kung Fu")

while True:
    data = in_file.read(16)
    if len(data) == 0:
        break

    encrypted_data = encrypt(data,round_keys)
    out_file.write(encrypted_data.encode())

in_file.close()
out_file.close()