import socket
from pygame import mixer
import time




s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# 绑定端口:
s.bind(('0.0.0.0', 20000))

mixer.init()
mixer.music.load('./1.mp3')
mixer.music.play()

while True:
    # 接收数据:
    data = b'';
    data, addr = s.recvfrom(1024)
    # print('Received from %s:%s.' % addr)
    # reply = 'Hello, %s!' % data.decode('utf-8')
    # s.sendto(reply.encode('utf-8'), addr)
    print(data)
    if(b'bj' == data):
        mixer.init()
        mixer.music.load('./1.mp3')
        mixer.music.play()
        # time.sleep(3 * 60 + 40)
        while(True):
            if(input('电梯内有人报警，请及时查看实时监控，是否关闭振铃(y/n)：') == 'y'):
                break
        mixer.music.stop()
        print('mmp')

