import asyncio
import socket
import aiocoap
from aiocoap import *
import aiocoap.resource as resource


class LatencyResource(resource.Resource):
    async def render_put(self, request):
        print("latency")
        return aiocoap.Message(code=aiocoap.CHANGED, payload=request.payload)

    async def render_get(self, request):
        return aiocoap.Message(payload=request.payload)


class ThroughputResource(resource.Resource):
    async def render_put(self, request):
        print("Throughput")
        return aiocoap.Message(code=aiocoap.CHANGED, payload=request.payload)

    async def render_get(self, request):
        return aiocoap.Message(payload=request.payload)


class PacketLossResource(resource.Resource):
    async def render_put(self, request):
        print("Packet loss")
        return aiocoap.Message(code=aiocoap.CHANGED, payload=request.payload)

    async def render_get(self, request):
        return aiocoap.Message(payload=request.payload)


def get_local_ip():
    """
    Get the local IP address of the machine
    """
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    local_ip = s.getsockname()[0]
    s.close()
    return local_ip


async def main():
    root = resource.Site()
    root.add_resource(['latency'], LatencyResource())
    root.add_resource(['throughput'], ThroughputResource())
    root.add_resource(['packetloss'], PacketLossResource())
    await Context.create_server_context(bind=(get_local_ip(), 5683), site=root)

    print(f"CoAP server running at coap://{get_local_ip()}:5683")
    await asyncio.get_running_loop().create_future()


if __name__ == '__main__':
    asyncio.run(main())
