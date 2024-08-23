from google.oauth2 import service_account
from googleapiclient.discovery import build
from googleapiclient.http import MediaIoBaseDownload
import io
import os

class PictureGet:
    def __init__(self):
        # サービスアカウントの認証情報ファイルのパス
        SERVICE_ACCOUNT_FILE = 'red-charger-428500-m6-31e7d82fa2ae.json'

        # 認証スコープ
        SCOPES = ['https://www.googleapis.com/auth/drive.readonly']

        # 認証情報を読み込む
        creds = service_account.Credentials.from_service_account_file(SERVICE_ACCOUNT_FILE, scopes=SCOPES)

        # Google Drive APIサービスを構築
        self.service = build('drive', 'v3', credentials=creds)

        # 画像ファイルを検索したいフォルダのIDを指定
        self.folder_id = '1CXXDJlczHhUTVbJVJ3-OFXXEILBwZcsr'  # ここにフォルダIDを入力
        


    def download(self,target_filename):
        # 検索したい画像ファイルの名前を指定
        
        # クエリを使用して、フォルダ内の特定の名前の画像ファイルをフィルタリング
        query = f"'{self.folder_id}' in parents and name='{target_filename}' and (mimeType='image/jpeg' or mimeType='image/png')"

        # ファイル情報を取得
        results = self.service.files().list(
            q=query,
            fields="files(id, name, mimeType)"
        ).execute()

        items = results.get('files', [])

        if not items:
            print(f"'{target_filename}' という名前の画像ファイルがフォルダ内に見つかりませんでした。")
        else:
            for item in items:
                print(f"名前: {item['name']}, ID: {item['id']}, MIMEタイプ: {item['mimeType']}")

                #ここからダウンロード
                download_folder = "picture"
                filename = item['name']
                file_id = item['id']

                #フォルダがない場合は作成
                if not os.path.exists(download_folder):
                    os.makedirs(download_folder)

                file_path = os.path.join(download_folder,filename)

                #Google Driveからファイルをダウンロード
                request = self.service.files().get_media(fileId=file_id)
                fh = io.BytesIO()

                # ダウンローダーを使用してファイルを取得
            downloader = MediaIoBaseDownload(fh, request)
            done = False
            while not done:
                status, done = downloader.next_chunk()
                print(f"Download {int(status.progress() * 100)}%.")

            # バイトデータをJPEGファイルとして保存
            with open(file_path, 'wb') as f:
                fh.seek(0)
                f.write(fh.read())

            print(f'ファイルが {file_path} に保存されました。')

