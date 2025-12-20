from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import OVSController
from mininet.link import TCLink
from mininet.cli import CLI

class BinaryTree7(Topo):
    def build(self):
        # Switches
        s1 = self.addSwitch('s1')  # root
        s2 = self.addSwitch('s2')
        s3 = self.addSwitch('s3')
        s4 = self.addSwitch('s4')
        s5 = self.addSwitch('s5')
        s6 = self.addSwitch('s6')
        s7 = self.addSwitch('s7')

        # Hosts
        h1 = self.addHost('h1')
        h2 = self.addHost('h2')
        h3 = self.addHost('h3')
        h4 = self.addHost('h4')

        # Binary tree links
        self.addLink(s1, s2)
        self.addLink(s1, s3)

        self.addLink(s2, s4)
        self.addLink(s2, s5)

        self.addLink(s3, s6)
        self.addLink(s3, s7)

        # Attach hosts to leaf switches
        self.addLink(h1, s4)
        self.addLink(h2, s5)
        self.addLink(h3, s6)
        self.addLink(h4, s7)

if __name__ == '__main__':
    topo = BinaryTree7()
    net = Mininet(topo=topo, controller=OVSController, link=TCLink)
    net.start()
    CLI(net)
    net.stop()
