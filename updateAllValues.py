from toGoogleDrive import SQLtoGoogleDrive
from SQLcode import SQLconnect
SQL = SQLconnect("0.0.0.0", "pi", "raspberry")
googleSheet = SQLtoGoogleDrive("SQLspreadsheet")
def updateSQLValues(time):
	for i in range(len(SQL.getPressure().fetchall())):
		if i == 0 or (i % time) == 0:
			pressure = getPressure().fetchall()
			temp = getTemp().fetchall()
			mass = getMass().fetchall()
			humid = getHumid().fetchall()
			date = getDate().fetchall()
			googleSheet.update(i+2, pressure[i][0], temp[i][0], mass[i][0], humid[i][0], date[i][0])
if __name__ == "__main__":
	updateSQLValues(1440)
