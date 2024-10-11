import React from 'react';
import {useState} from 'react';
import {Line} from 'react-chartjs-2';
import Header from './Header';
import './Ranking.css';
import './point.css';
import first from "../images/first.png";
import second from "../images/second.png";
import third from "../images/third.png";
import {Chart, registerables} from "chart.js";
Chart.register(...registerables);

function Graph(props){
    const classKeys = Object.keys(props.point);
    let labels = Object.keys(props.point["1-1"]);

    let classIndex = 0;
    if(parseInt(props.Class/10) == 1){
        classIndex = props.Class%10-1;
    } else{
        classIndex = parseInt(props.Class/10-1)*5-2 + props.Class%10;
    }

    const data = props.point[classKeys[classIndex]];

    const graphData = {
        labels : labels,
        datasets: [
            {
                label:props.Class,
                data:data,
                borderColor:"turquoise",
                lineTension: 0.3,
            },
        ],
    };

    const options = {
        responsive: true,
        maintainAspectRatio: false,
        aspectRatio: 2,
        plugins: {
            legend : {
                display: false,
            },
        },
        layout: {
            padding: {
                bottom: 0,
            }
        },
    };

    return (
        <div className="graph">
            <Line
                data={graphData}
                options={options}
                height="250"
            />
        </div>
    );
}

function Point(props) {
    let [choicedClass,setChoicedClass] = useState(11);
    let [className,setClassName] = useState("1年1組");

    function updateClass(){
        let grade = parseInt(choicedClass / 10);
        let Class = choicedClass % 10;
        if(grade !== 1){
            let classes = ["M","E","I","C","Z"];
            Class = classes[Class-1];
        }
        let a = String(grade) + "年" + String(Class) + "組";
        setClassName(a);
    }

    function changeClass(grade , Class){
        setChoicedClass(grade * 10 + Class);
        updateClass();
    }

    return(
        <div className="pointView">
            <h1 className="title">{className}のポイント</h1>
            <div className="point">
                <div className="graphArea">
                    <Graph Class={choicedClass} point={props.point} />
                </div>
                <div className="toggleList">
                    <input type="checkbox" id="toggle1" />
                    <label htmlFor="toggle1">▼一年<br /></label>
                    <div id="toggle-inner1">
                        <p onClick={() => changeClass(1,1)}>　1組</p>
                        <p onClick={() => changeClass(1,2)}>　2組</p>
                        <p onClick={() => changeClass(1,3)}>　3組</p>
                        <p onClick={() => changeClass(1,4)}>　4組</p>
                    </div>
                    <input type="checkbox" id="toggle2" />
                    <label htmlFor="toggle2">▼二年<br /></label>
                    <div id="toggle-inner2">
                        <p onClick={() => changeClass(2,1)}>　M</p>
                        <p onClick={() => changeClass(2,2)}>　E</p>
                        <p onClick={() => changeClass(2,3)}>　I</p>
                        <p onClick={() => changeClass(2,4)}>　C</p>
                        <p onClick={() => changeClass(2,5)}>　Z</p>
                    </div>
                    <input type="checkbox" id="toggle3" />
                    <label htmlFor="toggle3">▼三年<br /></label>
                    <div id="toggle-inner3">
                        <p onClick={() => changeClass(3,1)}>　M</p>
                        <p onClick={() => changeClass(3,2)}>　E</p>
                        <p onClick={() => changeClass(3,3)}>　I</p>
                        <p onClick={() => changeClass(3,4)}>　C</p>
                        <p onClick={() => changeClass(3,5)}>　Z</p>
                    </div>
                    <input type="checkbox" id="toggle4" />
                    <label htmlFor="toggle4">▼四年<br /></label>
                    <div id="toggle-inner4">
                        <p onClick={() => changeClass(4,1)}>　M</p>
                        <p onClick={() => changeClass(4,2)}>　E</p>
                        <p onClick={() => changeClass(4,3)}>　I</p>
                        <p onClick={() => changeClass(4,4)}>　C</p>
                        <p onClick={() => changeClass(4,5)}>　Z</p>
                    </div>
                    <input type="checkbox" id="toggle5" />
                    <label htmlFor="toggle5">▼五年<br /></label>
                    <div id="toggle-inner5">
                        <p onClick={() => changeClass(5,1)}>　M</p>
                        <p onClick={() => changeClass(5,2)}>　E</p>
                        <p onClick={() => changeClass(5,3)}>　I</p>
                        <p onClick={() => changeClass(5,4)}>　C</p>
                        <p onClick={() => changeClass(5,5)}>　Z</p>
                    </div>
                </div>
            </div>
        </div>
    )
}

function Ranking(props){
    const point = props.fetchData.point;

    let gradePoint = [0,0,0,0,0];
    const gradeList = ["1","2","3","4","5"];
    const keys = Object.keys(point);
    keys.map(key => {
        for(let i = 0 ; i < gradeList.length ; i++ ){
            if(key.indexOf(gradeList[i]) == 0){
                gradePoint[i] += point[key];
            }
        }
    })

    let firstKey = 0 , secondKey = 1 , thirdKey = 2;
    for(let i = 1 ; i < 5 ; i++){
        if(gradePoint[firstKey] < gradePoint[i]){
            thirdKey = secondKey;
            secondKey = firstKey;
            firstKey = i;
        } else if (gradePoint[secondKey] < gradePoint[i]){
            thirdKey = secondKey;
            secondKey = i;
        } else if (gradePoint[thirdKey] < gradePoint[i]){
            thirdKey = i;
        }
    }

    return(
        <div className="rankingView">
            <Header />
            <h1 className="title">夏の冷房祭り</h1>
            <div className="Ranking">
                <div className="rankingArea">
                    <div className="Rank">
                        <div id="first" className="rank">
                            <img src={first} width="100vw" />
                            <p>{firstKey+1}年生　{gradePoint[firstKey]}ポイント</p>
                        </div>
                        <div id="second" className="rank">
                            <img src={second} width="90vw" />
                            <p>{secondKey+1}年生　{gradePoint[secondKey]}ポイント</p>
                        </div>
                        <div id="third" className="rank">
                            <img src={third} width="80vw" />
                            <p>{thirdKey+1}年生　{gradePoint[thirdKey]}ポイント</p>
                        </div>
                    </div>
                </div>
            </div>
            <Point point={props.fetchPoint} />
        </div>
    )
}

export default Ranking;