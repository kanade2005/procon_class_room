import gspread
from oauth2client.service_account import ServiceAccountCredentials

class SpreadSheet:
    def __init__(self):
        # 認証情報の設定
        #本来はスプレッドシートのAPIの情報を設定する
        #今回は公開できないため以下のように対処
        scope = "認証情報"
        self.creds = "認証情報"
    
    #こちらはカメラ用に作成している
    def get_latest_data_front(self,sheet_name,row,sheetIndex):
        client = gspread.authorize(self.creds)
        # スプレッドシートの取得
        spreadsheet = client.open(sheet_name)
        # ワークシートの取得
        worksheet = spreadsheet.worksheet(sheetIndex)
        # クライアントの作成

        last_row = worksheet.cell(2,row).value

        return last_row

    def write_latest_data(self,sheet_name,sheetIndex,data):
        client = gspread.authorize(self.creds)
        spreadsheet = client.open(sheet_name)
        worksheet = spreadsheet.worksheet(sheetIndex)
        


    #最新のデータをviewから持ってくる
    def get_latest_data_view(self,sheet_name,sheetIndex):
        client = gspread.authorize(self.creds)
        spreadsheet = client.open(sheet_name)
        worksheet = spreadsheet.worksheet(sheetIndex)

        column1 = worksheet.col_values(12)
        column2 = worksheet.col_values(13)

        min_length = min(len(column1),len(column2))

        keys = column1[:min_length]
        values = column2[:min_length]

        data_dict = dict(zip(keys,values))

        return data_dict
    
    #weekデータをviewから持ってくる
    def get_week_data_view(self,sheet_name,sheetIndex):
        client = gspread.authorize(self.creds)
        spreadsheet = client.open(sheet_name)
        worksheet = spreadsheet.worksheet(sheetIndex)

        count = 0
        header = worksheet.row_values(1)
        data = worksheet.get_all_values()[1:]

        for i in header:
            if i == '':
                del header[count:]
                break
            count = count + 1

        print(header)
        result = {}

        for col_index,key in enumerate(header[1:],start=1):
            result[key] = {}

            for row in data:
                date = row[0]
                value = row[col_index]

                if date:
                    result[key][date] = value

        return result
    