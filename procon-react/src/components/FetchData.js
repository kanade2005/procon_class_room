function fetchGet(){
    let fetchData = undefined;

    fetch("http://127.0.0.1:5050/data")
        .then(response => response.json())
        .then(data => fetchData = data)
        .catch(error => console.error("フェッチ失敗! ちっくしょー\n",error));

        if(fetchData == undefined){
            fetchData = {
                "checkTemp": {
                "1-1": 1,
                "1-2": 1,
                "1-3": 1,
                "1-4": 1,
                "2I": 1,
                "3I": 1,
                "4I": 1,
                "5I": 1
            },
            "light": {
                "1-1": false,
                "1-2": false,
                "1-3": false,
                "1-4": false,
                "2I": true,
                "3I": false,
                "4I": false,
                "5I": false
            },
            "person_number": {
                "1-1": 7,
                "1-2": 1,
                "1-3": 0,
                "1-4": 0,
                "2I": 0,
                "3I": 0,
                "4I": 12,
                "5I": 0
            },
            "point": {
                "1-1": 17,
                "1-2": 17,
                "1-3": 17,
                "1-4": 17,
                "2I": 19,
                "3I": 20,
                "4I": 18,
                "5I": 10
            },
            "temp_standard": 18.2,
            "tempearture": {
                "1-1": "24",
                "1-2": "25",
                "1-3": "30",
                "1-4": "31",
                "2I": "32",
                "3I": "33",
                "4I": "26",
                "5I": "33"
            }
        }
    }

    console.log(fetchData);
    return fetchData;
}

export default fetchGet;