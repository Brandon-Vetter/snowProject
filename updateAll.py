import mainPiCode
from SQLcode import SQLconnect
SQL = SQLconnect("0.0.0.0", "pi", "raspberry" )

usrInput = int(input("1 for update everyting then exit, 2 to run update everything then main code, 3 to start main function with time of 60, 4 to start main file as normal: "))
if usrInput == 1:
	print(len(SQL.getHumid().fetchall()))
	startingPos = int(input("Enter how far back you want it to run"))
	mainPiCode.updateAll(startingPos)
	print("updated data")
elif usrInput == 2:
	startingPos1 = int(input("Enter how far back you want it to run"))
	mainPiCode.updateAll(startingPos1)
	print("updated data, now running main function")
	mainPiCode.main(60)
elif usrInput == 3:
	mainPiCode.main(60)
elif usrInput == 4:
	mainPiCode.main(86400)
