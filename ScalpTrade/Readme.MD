# ScalpTrade
	
ScalpTrade.cpp is Entry point to the Program Solution

## Version

* C++ : 14

## Steps To Run Executable

* Run the make file in the Source Direcotry using below Command 

```bash
make
```

* Run the Executable file created by Make using the below command

```bash
./ScalpTrade IBM B 100 1 127.0.0.1 14000 127.0.0.1 15000 
```



## Design 

* Command Line Inputs are parsed and Stored in 

## Assumptions

* Implemented Has a Relationship instead of byte slicing to get specific bytes for each message type
* As the Header is small. Decided to not slice the input stream based on the size of the header. 
* Assumed only two message types will be received in the
* utillized a do while loop as the Vwap value needs to be caluated only if atlease on value is present. 
* Quantity is assumed to be a non zero value. 
* Return value from read() method in sockets is used to Check if Socket connection is alive.


## Known Issues

## Unit Testing

* TBD

## Integration Testing

* TBD

## Performance Testing

* TBD







