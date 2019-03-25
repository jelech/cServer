import os

username = "frank"
new_file_name = username + ".cpp"
command = "g++ "+ new_file_name + " -o " + username

os.system(command)
command = "cat in.txt | ./" + username + " > out.txt"
os.system(command)

