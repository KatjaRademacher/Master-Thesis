The automation tool for running traffic light simulations with SUMO 
needs the following elements to run:
 - Qt
 - Visual Studio 2012 (is used here)
 - a database with the vehicle count data 
 - python
 
 
 The following folders are set:
- masterthesis32: 
	is the 32bit application folder with the visual studio solution and the source code

- Simulation:
	holds the intersection data with the network files and so on needed to run the SUMO simulations
	
	/tools
		this holds the adaptive traffic light program (python script)that is run 
		also includes python scripts needed to convert the output data
 