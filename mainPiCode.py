from toGoogleDrive import SQLtoGoogleDocs as googleDocs
from SQLcode import SQLconnect as sql
import sys
from datetime import timedelta
import time
def main(ltime):
	while True:
		try:
			GoogleDocs = googleDocs("SQLspreadsheet")
			Sql = sql("0.0.0.0", "pi", "raspberry")
			pressure = Sql.getPressure().fetchall()
			humid = Sql.getHumid().fetchall()
			temp = Sql.getTemp().fetchall()
			mass = Sql.getMass().fetchall()
			date = Sql.getDate().fetchall()
			timec = Sql.getTime().fetchall()

			pressureLast = pressure[len(pressure)-1][0]
			humidLast = humid[len(humid)-1][0]
			tempLast = temp[len(temp)-1][0]
			massLast = mass[len(mass)-1][0]

			dateLast =str(date[len(date)-1][0] - timedelta(hours = 8))
			timeLast =str(timec[len(date)-1][0] - timedelta(hours = 8))

			GoogleDocs.update(GoogleDocs.findLongestColumn()+1, pressureLast, humidLast, tempLast, massLast, dateLast, timeLast)
			Sql.closeConn()
			time.sleep(ltime)
		except:
			print("an error has accured")
			print(sys.exc_info())
			time.sleep(ltime)
def updateAll(startingPos):
	GoogleDocs = googleDocs("SQLspreadsheet")
	Sql = sql("0.0.0.0", "pi", "raspberry")
	counter = 0
	for i in range(len(Sql.getHumid().fetchall()) - startingPos , len(Sql.getHumid().fetchall())):
		date = str(Sql.getDate().fetchall()[i][0] - timedelta(hours = 8))
		timec = str(Sql.getTime().fetchall()[i][0] - timedelta(hours = 8))
		GoogleDocs.update(GoogleDocs.findLongestColumn()+1, Sql.getPressure().fetchall()[i][0], Sql.getHumid().fetchall()[i][0], Sql.getTemp().fetchall()[i][0], Sql.getMass().fetchall()[i][0], date, timec)
		if counter == 8:
			counter = 0
			time.sleep(60)
		counter+=1
if __name__ == "__main__":
#	main(86400)
	main(70)
