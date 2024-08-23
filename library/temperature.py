import requests

class Temperature:
    def __init__(self):
        self.api_key = 'c0384c67ee7348139a491228242008'
        self.city_name = "Tokushima"
        

    def get_temperature(self):
        base_url = "http://api.weatherapi.com/v1/current.json"

        params = {
            'key' : self.api_key,
            'q' : self.city_name,
            'api' : 'no'
        }

        response = requests.get(base_url,params=params)

        data = response.json()

        if response.status_code == 200:
            temperature = data['current']['temp_c']
            #print(f"The temperature in {city_name} is {temperature}")
            return temperature
        else:
            return "error"