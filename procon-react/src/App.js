import {BrowserRouter, Link, Route, Routes} from "react-router-dom";
import Home from './components/home';
import Ranking from './components/Ranking';
import Destination from './components/Destination';
import fetchGet from './components/FetchData';
import fetchPoint from './components/FetchPoint';
import './App.css';

function App(){
    const data = fetchGet();
    const point = fetchPoint();

    return (
        <BrowserRouter>
        <div>
            <Link className="AppLink" to="/">ポイント</Link>
            <Link className="AppLink" to="/Ranking">ランキング</Link>
            <Link className="AppLink" to="/Destination">行き先</Link>
            <Routes>
                <Route path="/" element={<Home fetchData={data}/>} />
                <Route path="/Ranking" element={<Ranking fetchData={data} fetchPoint={point}  />} />
                <Route path="/Destination" element={<Destination fetchData={data} />} />
            </Routes>
        </div>
        </BrowserRouter>
    )
}

export default App;