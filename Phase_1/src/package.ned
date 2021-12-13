package bitmanuplations;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=468.62497,295.03");
    types:
    submodules:
        node0: Sender {
            @display("p=138,38;i=block/circle");
        }
        node1: Receiver {
            @display("p=289.32498,37.489998;i=block/circle");
        }
        coordinator: Coordinator {
            @display("p=211.9,139.36499;i=block/cogwheel");
        }
    connections:
        node0.out --> node1.in;
        node1.out --> node0.in;
        coordinator.out0 --> node0.coordination;
        coordinator.out1 --> node1.coordination;
}

