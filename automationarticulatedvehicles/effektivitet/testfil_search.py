# Från GPT, funkar

import traci

# Ange sökvägen till din SUMO-konfigurationsfil
sumo_cfg = "osm_2.sumocfg"

# Starta SUMO med TraCI
traci.start(["sumo", "-c", sumo_cfg])

# Ange start- och målpunkt (dessa ska vara nod-ID:n från din SUMO-fil)
start_edge1 = "-27302749#6"
end_edge1 = "27302768#1"

start_edge2 = "-27302749#6"
end_edge2 = "27302768#1"

# Hämta kortaste vägen
route1 = traci.simulation.findRoute(start_edge1, end_edge1)
route2 = traci.simulation.findRoute(start_edge2, end_edge2)

'''
# Skriv ut den kortaste vägen
print("Kortaste vägen:")
print(route1)
for edge in route1.edges:
    print(edge)

print("Kortaste vägen 2:")
print(route2)
for edge in route2.edges:
    print(edge)
'''

# Stäng TraCI
traci.close()


# Starta SUMO med TraCI
sumoCmd = ["sumo-gui", "-c", sumo_cfg]
traci.start(sumoCmd)

# Döp bil och rutt
vehicle_id1 = "test_vehicle1"
route_id1 = "test_route1"

vehicle_id2 = "test_vehicle2"
route_id2 = "test_route2"

# Lägg till rutter och bilar
traci.route.add(route_id1, route1.edges)
traci.vehicle.add(vehicle_id1, route_id1)
traci.route.add(route_id2, route2.edges)
traci.vehicle.add(vehicle_id2, route_id2)

step = 0
# Kör simuleringen
while True:
    traci.simulationStep()
    sumo_cfg = "osm_2.sumocfg"

    # Hämta kortaste vägen
    for vehicle in traci.vehicle.getIDList():
            route = traci.vehicle.getRoute(vehicle)
            new_route = traci.simulation.findRoute(traci.vehicle.getRoadID(vehicle), route[-1])
            traci.vehicle.setRoute(vehicle, new_route.edges)

    # Testar att stoppa en bil
    if step == 18:
        traci.vehicle.setSpeed(vehicle_id1, 0)
    elif step == 100:
        traci.vehicle.setSpeed(vehicle_id1, -1)

    # Bryt när det inte finns bilar kvar i simuleringen
    if traci.vehicle.getIDCount() <= 0:
        traci.close()
    step += 1
#traci.close()
