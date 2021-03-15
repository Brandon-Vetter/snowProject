import mysql.connector
class SQLconnect():
	def __init__(self, host, username, password):
		self.host = host
		self.username = username
		self.password = password
		self.mydb = mysql.connector.connect(host=self.host, user=self.username, password=self.password)
		self.cursor = self.mydb.cursor(buffered=True)
	def getPressure(self):
		self.cursor.execute('SELECT pressure FROM weatherData.weatherData;')
		return self.cursor
	def getTemp(self):
		self.cursor.execute('SELECT temp FROM weatherData.weatherData;')
		return self.cursor
	def getMass(self):
		self.cursor.execute('SELECT mass FROM weatherData.weatherData;')
		return self.cursor
	def getHumid(self):
		self.cursor.execute('SELECT humid FROM weatherData.weatherData;')
		return self.cursor
	def getDate(self):
		self.cursor.execute('SELECT Date FROM weatherData.weatherData;')
		return self.cursor
	def getTime(self):
		self.cursor.execute('SELECT time FROM weatherData.weatherData;')
		return self.cursor
	def closeConn(self):
		self.cursor.close()
if __name__ == "__main__":
	sql = SQLconnect('0.0.0.0', 'pi', 'raspberry')
	testvar = sql.getHumid().fetchall()
	print(testvar[0][0])
