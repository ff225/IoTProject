import socket

import paho.mqtt.client as mqtt

# broker_addr = "192.168.1.54"
broker_addr = "192.168.254.38"
broker_port = 1883

latency_topic = "room/devices/esp32/latency"
latency_topic_ack = "room/devices/esp32/latency_ack"

throughput_topic = "room/devices/esp32/throughput"
throughput_topic_ack = "room/devices/esp32/throughput_ack"

packet_loss = "room/devices/esp32/packet_loss"
packet_loss_ack = "room/devices/esp32/packet_loss_ack"

'''
Funzione che gestisce la connessione ad mqtt
'''


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("connected to mqtt broker")
    else:
        print(f"connection failed with error code {rc}")


'''
mqtt callback
'''


def on_message(client, userdata, message):
    content = message.payload.decode('utf-8')
    if message.topic == latency_topic:
        print(f"latency_topic, message content: {content}")

        client.publish(latency_topic_ack, content, qos=1)

    if message.topic == throughput_topic:
        print(f"throughput_topic, message content: {content}")
        # client.publish(throughput_topic_ack, content, qos=1)

    if message.topic == packet_loss:
        print("packet_loss, messaggio ricevuto")
        client.publish(packet_loss_ack, content, qos=1)


def subscribe(client, topic):
    if isinstance(topic, list):
        for t in topic:
            print(f"subscribing to {t}")
            client.subscribe(t)
    else:
        client.subscribe(topic)


def main():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    mqtt_broker_ip = socket.gethostbyname("MacBook-Pro-di-Francesco-3.local")
    client.connect(mqtt_broker_ip, broker_port)
    topics_to_subscribe = [latency_topic, throughput_topic, packet_loss]
    subscribe(client, topics_to_subscribe)
    # time.sleep(5)
    client.loop_forever()


if __name__ == '__main__':
    main()
