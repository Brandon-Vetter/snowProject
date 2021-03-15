import sqlite3
import gspread
from oauth2client.service_account import ServiceAccountCredentials
connection = sqlite3.connect
class SQLtoGoogleDocs:
    def __init__(self, doc):
	# the initiation of the code sets up its ability to edit the google spreadsheet doc is the document
        self.scope = ['https://www.googleapis.com/auth/drive']
        self.creds = ServiceAccountCredentials.from_json_keyfile_name('piCode.json',self.scope)
        self.client = gspread.authorize(self.creds)
        self.sheet = self.client.open(doc).sheet1
    def printAllValues(self):
        print(self.sheet.get_all_records())
    # ref is going from A1 down, so if you input 4, it would start at A4
    # *args is what is inputted in the rows, so if your referance is 4, and you are inputting 2,3,4,5 it would go A4 would have 2, B4 would have 3 ect
    def update(self, ref, *args):
        self.ref = 'A' + str(ref)
        self.sheet.update(self.ref, [args])
    def getPressure(self):
        self.sheet.col_values(1)
    def getHumid(self):
        self.sheet.col_values(2)
    def getTemp(self):
        self.sheet.col_values(3)
    def getMass(self):
        self.sheet.col_values(4)
    def findLongestColumn(self):
        pressureLength = len(self.sheet.col_values(1))
        humidLength = len(self.sheet.col_values(2))
        tempLength = len(self.sheet.col_values(3))
        massLength = len(self.sheet.col_values(4))
        maxListLength = 0
        colLengths = [pressureLength, humidLength, tempLength, massLength]
        for i in colLengths:
            if i > maxListLength:
                maxListLength = i
        return maxListLength
if "__main__" == __name__:
     converter = SQLtoGoogleDocs("SQLspreadsheet")
     converter.update(2,2,3,4,5,"TEST")
     converter.printAllValues()
     print(converter.findLongestColumn())
