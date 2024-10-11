import React, { useEffect, useState, useCallback } from 'react';

import Header from './Header';
import './home.css'

import useEmblaCarousel from 'embla-carousel-react';
import Autoplay from 'embla-carousel-autoplay'
import {NextButton, PrevButton, usePrevNextButtons} from './EmblaCarouselArrowButtons'

import {Bar} from 'react-chartjs-2';
import {Chart, registerables} from "chart.js";
Chart.register(...registerables);


function Graph(props){
    let labels = [];
    const fetchClassList = Object.keys(props.temp);
    const graphData = [];

    if(props.grade === 1) {
        for(let i = 0 ; i < 4 ; i++){
            labels.push((i+1) + "組");
            graphData.push(props.temp[fetchClassList[i]]);
        }
    } else {
        const labelTemp = ["M","E","I","C","Z"];
        for(let i = 0 ; i < labelTemp.length ; i++){
            labels.push(props.grade + labelTemp[i]);
            graphData.push(props.temp[fetchClassList[i+(props.grade-1)*5-1]]);
        }
    }

    const options = {
        title: {
            display : true,
            position : "top",
            fontSize  : 14,
            text : props.title
        },
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

    const data = {
        labels : labels,
        datasets: [
            {
                data:graphData,
                borderColor:"turquoise",
                lineTension: 0.3,
            },
        ],
    };

    return(
        <Bar 
            options={options}
            data={data}
            width="320"
        />
    )
}



function Carousel(props){
    const slides = props.slides;
    const options = props.options;
    const temperature = props.fetchData["tempearture"];

    const [emblaRef, emblaApi] = useEmblaCarousel(options, [
        Autoplay({ playOnInit: true, delay: 8000 })
    ])
      
    const {
        prevBtnDisabled,
        nextBtnDisabled,
        onPrevButtonClick,
        onNextButtonClick
    } = usePrevNextButtons(emblaApi)
      
    const onButtonAutoplayClick = useCallback(
        (callback) => {
            const autoplay = emblaApi?.plugins()?.autoplay
            if (!autoplay) return
      
            const resetOrStop = autoplay.play
      
            resetOrStop()
            callback()
          },
          [emblaApi]
        )
      
    return(
        <div className="embla">
            <div className="embla__viewport" ref={emblaRef}>
                <div className="embla__container">
                    {slides.map((index) => (
                        <div className="embla__slide" key={index}>
                            <div className="embla__slide__number">
                                <Graph grade={index+1} temp={temperature} />
                            </div>
                        </div>
                    ))}
                </div>
            </div>

            <div className="embla__controls">
                <div className="embla__buttons">
                    <PrevButton
                        onClick={() => onButtonAutoplayClick(onPrevButtonClick)}
                        disabled={prevBtnDisabled}
                    />
                    <NextButton
                        onClick={() => onButtonAutoplayClick(onNextButtonClick)}
                        disabled={nextBtnDisabled}
                    />
                </div>
            </div>
        </div>
    )
}

function Slider(){
    return (
        <div className="slider">
            <div className="slider_content">
                <div><p>現在の1位は1年の60ポイント!</p></div>
                <div><p>昨日一番節電できていたのは2E</p></div>
                <div><p>今日は3Mに集まろう!</p></div>
            </div>
        </div>
    )
}

function Home(props){
    const OPTIONS = {loop : true}
    const SLIDES = Array.from(Array(5).keys())

    return (
        <div className="homeView">
            <Header/>
            <Slider />
            <Carousel slides={SLIDES} options={OPTIONS} fetchData={props.fetchData} />
        </div>
    )
}

export default Home;