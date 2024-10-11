import React from 'react';
import {Link} from "react-router-dom";
import './Header.css';
import icon from '../images/icon.png'
import {useMediaQuery} from 'react-responsive';

function PC(){
    return (
        <div className="header">
            <img src={icon} alt="アイコン" className="icon" />
            <nav>
                <Link to="/">ホーム</Link>
                <Link to="/Destination">移動先</Link>
                <Link to="/Ranking">ランキング</Link>
            </nav>
        </div>
    )
}

function Phone(){
    return (
        <div className="header">
            <img src = {icon} alt="アイコン" className="icon" />
            <div className="nav">
                <input id="drawer-input" className="drawer-hidden" type="checkbox" />
                <label htmlFor="drawer-input" className="drawer-open"><span></span></label>
                <nav className="nav-content">
                    <h3> </h3>
                    <ul className="nav-list">
                        <li className="navitem"><Link to="/">ポイント</Link></li>
                        <li className="navitem"><Link to="/Destination">移動先</Link></li>
                        <li className="navitem"><Link to="/Ranking">ランキング</Link></li>
                    </ul>
                </nav>
            </div>
        </div>
    )
}

function Header(){
    const isPhone: boolean = useMediaQuery({query: "(max-width : 600px)"});
    return(
        <div className="headerView">
            {isPhone && <Phone />}
            {!isPhone && <PC />}
        </div>
    )
}

export default Header;