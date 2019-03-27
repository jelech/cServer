import redis
import time
r = redis.Redis(host='127.0.0.1', port=6379)
r.lpush("a","d")
checkFileDir = "checkProblem/00001/"
compiledFileName = "frank123"
username = "frank"
r.lpush("running_queue", "%s+%s+%s+%f"%(checkFileDir, compiledFileName, username, time.time()))
# string = r.lrange("running_queue", 0, 100)[0].decode()
# print(string.split('+'))
