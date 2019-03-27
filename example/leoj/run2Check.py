import os
import redis
import time
import subprocess

def run2Check():
    total_run_time = 0
    while True:
        print("total_run_time: ",total_run_time)
        redisDB = redis.Redis(host='127.0.0.1', port=6379)
        while redisDB.llen("running_queue") != 0:
            running_queue = redisDB.rpop("running_queue").decode()
            checkFileDir, compiledFileName, username, upTime = running_queue.split('+')

            command = "g++ %s.cpp -o %s" % (compiledFileName, checkFileDir + compiledFileName)
            subprocess.call(command, shell=True)

            print(total_run_time)
            total_run_time += 1

            commande = ("cd %s && cat in.txt | ./%s > %s.txt" % (checkFileDir, compiledFileName, compiledFileName))
            standout = subprocess.getoutput(commande)
            print(commande)
            
            commande = ("cd %s && ./check stout.txt %s.txt" % (checkFileDir, compiledFileName))
            standout = subprocess.getstatusoutput(commande)
            print(commande)

            commande = ("cd %s && rm %s && rm %s.txt" % (checkFileDir, compiledFileName, compiledFileName))
            subprocess.getstatusoutput(commande)
            print(commande)

            with open("db", 'a') as dbf:
                temp = str(time.time()) + "=>" + time.asctime(time.localtime(time.time())) + " <%s> : [%s]\n"% (username, standout)
                dbf.write(temp)

if __name__ == "__main__":
    run2Check()