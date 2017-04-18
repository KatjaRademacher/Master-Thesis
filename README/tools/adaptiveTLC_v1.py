'''
Created on 16.11.2016
adaptiveTLC Version 1.0
@author: Katja Rademacher
'''
# basic traffic lights logic and sequence
# <tlLogic id="int170" type="static" programID="0" offset="0">
#         0<phase duration="30" state="rrrrrrGGGGggrrrrrrGGGGgg"/>
#         1<phase duration="5"  state="rrrrrryyyyyyrrrrrryyyygg"/>
#         2<phase duration="30" state="GGGGggrrrrrrGGGGggrrrrrr"/>
#         3<phase duration="5"  state="yyyyyyrrrrrryyyyyyrrrrrr"/>
#     </tlLogic>
from __future__ import absolute_import
from __future__ import print_function

import os
import sys
import time
import optparse
import subprocess

# we need to import python modules from the $SUMO_HOME/tools directory
try:
    sys.path.append(os.path.join(os.path.dirname(
        __file__), '..', '..', '..', '..', "tools"))  # tutorial in tests
    sys.path.append(os.path.join(os.environ.get("SUMO_HOME", os.path.join(
        os.path.dirname(__file__), "..", "..", "..")), "tools"))  # tutorial in docs
    from sumolib import checkBinary
except ImportError:
    sys.exit(
        "please declare environment variable 'SUMO_HOME' as the root directory of your sumo installation (it should contain folders 'bin', 'tools' and 'docs')")

import traci
import sumolib
# the port used for communicating with your sumo instance
PORT = 8873
nsLoops = [] 
nsEdges = [] 
ewLoops = [] 
ewEdges = []
yellowPhaseDuration = 5
minDuration = 20
maxDuration = 60
greenPhaseDuration = 30
lastGreenPhaseDurations = {'NS': 30, 'EW': 30}
endTime = 3600
intersectionId = "int"

def read_NetworkData(simulationPath):
    fileBase = os.path.dirname(simulationPath)+ '/' + intersectionId
    ilArray = sumolib.sensors.inductive_loop.read(fileBase + "-loops25m.det.xml")
    network = sumolib.net.readNet(fileBase + "-adaptive.net.xml")
    for iLoop in ilArray:
        if ('north' in iLoop.id or 'south' in iLoop.id):
            nsLoops.append(iLoop.id)
            edgeId = network.getLane(iLoop.lane).getEdge().getID()
            if edgeId not in nsEdges:
                nsEdges.append(edgeId)
        else: 
            if ('east' in iLoop.id or 'west' in iLoop.id):
                ewLoops.append(iLoop.id)
                edgeId = network.getLane(iLoop.lane).getEdge().getID()
                if edgeId not in ewEdges:
                    ewEdges.append(edgeId)

def getNumberOfApproachingVehiclesFromIL_NS():
    vehicles = 0
    for loopId in nsLoops:
        vehicles += traci.inductionloop.getLastStepVehicleNumber(loopId)
    return vehicles

def getNumberOfApproachingVehiclesFromIL_EW():
    vehicles = 0
    for loopId in ewLoops:
        vehicles += traci.inductionloop.getLastStepVehicleNumber(loopId)
    return vehicles

def getEdgeDensityNS():
    density = 0
    for edgeId in nsEdges:
        density += traci.edge.getLastStepOccupancy(edgeId)
    return density

def getEdgeDensityEW():
    density = 0
    for edgeId in ewEdges:
        density += traci.edge.getLastStepOccupancy(edgeId)
    return density

def determineNextGreenPhase( activePhase, vehiclesEW, vehiclesNS):
    nextPhase = activePhase
    if activePhase == 2:
        if vehiclesEW >= vehiclesNS:
            nextPhase = 0
        else: 
            if (lastGreenPhaseDurations['NS'] >= maxDuration):
                nextPhase = 0
    else:
        if vehiclesEW <= vehiclesNS:
            nextPhase = 2
        else: 
            if (lastGreenPhaseDurations['EW'] >= maxDuration):
                nextPhase = 2
    return nextPhase    

def determinePhaseLength(activePhase, newGreenPhase, vehiclesEW, vehiclesNS):
    #determine phase length based on density on roads and stopping vehicles
    #densityNS = getEdgeDensityNS()
    #densityEW = getEdgeDensityEW()
    duration = minDuration
    #print("EW:", vehiclesEW, densityEW) 
    #print("NS:", vehiclesNS, densityNS)
    if (activePhase == newGreenPhase):
        if activePhase == 0: #EW
            duration /= vehiclesEW
        else: 
            duration /= vehiclesNS   
    if duration > maxDuration:
        duration = maxDuration   
    return int(duration)

def setLastGreenPhaseDuration(activePhase, nextPhase, duration):
    if nextPhase == 2:
        if nextPhase == activePhase:
            lastGreenPhaseDurations['NS'] += duration
        else: lastGreenPhaseDurations['NS'] = duration
    else: #nextPhase = 0
        if nextPhase == activePhase:
            lastGreenPhaseDurations['EW'] += duration
        else: lastGreenPhaseDurations['EW'] = duration

def run():
    vehVolumeNS = 0
    vehVolumeEW = 0
    totalVolumeEW = 0
    totalVolumeNS = 0
    activePhase = 0
    nextPhaseDetermination = 29
    setPhaseDurationTime = -1
    
    print(vehVolumeEW, vehVolumeNS, activePhase)
    #"""execute the TraCI control loop"""
    traci.init(PORT)
    step = 0
    # we start with phase 2 where EW has green
    #traci.trafficlights.setPhase("0", 2)
    while step < endTime:
        traci.simulationStep()
        vehVolumeEW += getNumberOfApproachingVehiclesFromIL_EW()
        vehVolumeNS += getNumberOfApproachingVehiclesFromIL_NS()  
        
        #get data
        if step == nextPhaseDetermination:
            activePhase = traci.trafficlights.getPhase(intersectionId)
            nextPhase = determineNextGreenPhase(activePhase, vehVolumeEW, vehVolumeNS)
            greenPhaseDuration = determinePhaseLength(activePhase, nextPhase, vehVolumeEW, vehVolumeNS)
            
            
            #reset vehicles volumes for approaching edges for chose phase
            if nextPhase == 0:
                totalVolumeEW += vehVolumeEW
                vehVolumeEW = 0
            else: 
                totalVolumeNS += vehVolumeNS
                vehVolumeNS = 0
            
            setLastGreenPhaseDuration(activePhase, nextPhase, greenPhaseDuration)
            print('Phase switch: -step',step, 'oldPhase', activePhase, 'newGreen:', nextPhase,'time:', greenPhaseDuration)
            
            #switch phases if needed or make active phase longer
            if not nextPhase == activePhase:
                #switch to the following yellow phase
                traci.trafficlights.setPhase(intersectionId, activePhase+1)
                setPhaseDurationTime = step + yellowPhaseDuration + 1
                nextPhaseDetermination += yellowPhaseDuration + greenPhaseDuration;
            else:
                traci.trafficlights.setPhaseDuration(intersectionId, greenPhaseDuration)
                nextPhaseDetermination += greenPhaseDuration
               
        if step == setPhaseDurationTime:
            traci.trafficlights.setPhaseDuration(intersectionId, greenPhaseDuration) 

        step += 1
    traci.close()
    print("EW:",vehVolumeEW,"NS:", vehVolumeNS)
    sys.stdout.flush()
    
def get_options():
    optParser = optparse.OptionParser()
    optParser.add_option("--gui", action="store_true",
                        default=False, help="run the commandline version of sumo")
    optParser.add_option("-e", "--end-time", dest="endtime",
                  help="define end time for simulation", type="int")
    optParser.add_option("-i", "--intersection-number", dest="intersection",
                  help="define the intersection for simulation", type="int")
    optParser.add_option("-c", "--sumo-config", dest="sumoConfig",
                  help="define the configuration file for SUMO for simulation", metavar="FILE")
    options, args = optParser.parse_args()
    return options

# this is the main entry point of this script
if __name__ == "__main__":
    options = get_options()

    # this script has been called from the command line. It will start sumo as a
    # server, then connect and run
    if options.gui:
        sumoBinary = checkBinary('sumo-gui')
    else:
        sumoBinary = checkBinary('sumo')
    
    endTime = options.endtime
    intersectionId += str(options.intersection)
    sumoConfig = options.sumoConfig
    testPath = os.path.dirname(sumoConfig)
    print(endTime, intersectionId, sumoConfig, testPath)

    # first, generate the route file for this simulation
    read_NetworkData(testPath)

    # this is the normal way of using traci. sumo is started as a
    # subprocess and then the python script connects and runs
    sumoProcess = subprocess.Popen([sumoBinary, "-c", sumoConfig, "--output-prefix=" + time.strftime("%H%M%S")+ "_adaptive_",
                                    "--remote-port", str(PORT)], stdout=sys.stdout, stderr=sys.stderr)
    run()
    sumoProcess.wait()