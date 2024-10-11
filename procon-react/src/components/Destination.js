import React, {useEffect, useState} from 'react';
import Header from './Header';
import './Destination.css';

function Tablecontent(props){
    const [fetchData,setFetchData] = useState(props.fetchData);

    useEffect(() => {
        fetch("http://127.0.0.1:5050/data")
            .then(response => response.json())
            .then(data => setFetchData(data))
            .catch(error => console.error("フェッチ失敗!\n",error));

            console.log(fetchData);
    },[]);

    return (
        <table>
            {
                (function (){
                    const table = [];
                    const thContent = [];
                    thContent.push(<th>クラス</th>);
                    thContent.push(<th>空調</th>);
                    thContent.push(<th>照明</th>);
                    thContent.push(<th>人数</th>);
                    table.push(<tr>{thContent}</tr>);
                    
                    if(fetchData !== undefined){
                        const fetchDataKeys = Object.keys(fetchData);
                        const classKeys = Object.keys(fetchData[fetchDataKeys[0]]);

                        for(let i = 0 ; i < classKeys.length ; i++){
                            const trContent = [];

                            trContent.push(<td>{classKeys[i]}</td>);

                            if(fetchData["checkTemp"][classKeys[i]] == 1){
                                trContent.push(<td>ON</td>);
                            } else {
                                trContent.push(<td>OFF</td>);
                            }

                            if(fetchData["light"][classKeys[i]] == true){
                                trContent.push(<td>ON</td>);
                            } else {
                                trContent.push(<td>OFF</td>);
                            }

                            trContent.push(<td>{fetchData["person_number"][classKeys[i]]}</td>);

                            table.push(<tr>{trContent}</tr>);
                        }
                    }
                    

                    return <tbody>{table}</tbody>;
                }())
            }
        </table>
    )
}

function Destination(props){
    let dest = [1,2,3,4,5];

    const list = props.fetchData["person_number"];
    let max  = Object.keys(list)[0];
    Object.keys(list).map(key => {
        if(list[max] < list[key]){
            max = key;
        }
    })

    const destClass = max;

    return(
        <div className="destinationView">
            <Header />
            <div>
                <h1>↓ 今日の移動先はこちら ↓</h1>
                <div className="destList">
                    <h2>1年生 → {destClass}へ移動</h2>
                    <h2>2年生 → {destClass}へ移動</h2>
                    <h2>3年生 → {destClass}へ移動</h2>
                    <h2>4年生 → {destClass}へ移動</h2>
                    <h2>5年生 → {destClass}へ移動</h2>
                    <br />
                    <h2>移動して節電しよう!!</h2>
                </div>
                <div className="space"></div>
                <Tablecontent fetchData={props.fetchData} />
                <div className="space"></div>
            </div>
        </div>
    )
}

export default Destination;