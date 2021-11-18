#UCR

__author__ = "\
Emmanuel Anchía Lobo B80445\
Diego Chinchilla Otárola B82227\
Fabián Alonso González Rojas B83493\
Rodrigo Li Qiu B94263"
import networkx as nx

ports = [1024, 1124, 1224, 1324, 1424, 1524, 1624, 1724, 1824, 1924]

edges =   [ ( 1224, 1324, {'hops':1} ),
            ( 1024, 1124, {'hops':1} ),
            ( 1124, 1224, {'hops':1} ),
            ( 1524, 1624, {'hops':1} ),
            ( 1624, 1724, {'hops':1} ),
            ( 1724, 1824, {'hops':1} ),
            ( 1824, 1924, {'hops':1} ),
            ( 1424, 1524, {'hops':1} ),
            ( 1924, 1024, {'hops':1} ),
            ( 1324, 1424, {'hops':1} ) ]

G = nx.Graph()

for i in ports:
    G.add_node(i)
G.add_edges_from(edges)

pos = nx.spiral_layout(G)


for i in ports:
    print(str(i) + ":")
    for k in ports:
        if i == k:
            continue
        else:
            length = nx.dijkstra_path_length(G,source=i,target=k,weight='hops')
            print(f"from {str(i)} to {str(k)} : " + str(length) + " hops")


