import React, {useState} from 'react';
import styles from './styles/Home.module.css'
interface LoginProps {
    setAuthenticated: Function,
    setUser: Function
}

const Login = (
    {setAuthenticated, setUser}: LoginProps) => {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');

    const handleLogin = () => {
        setAuthenticated(true);
        fetch('/api/login', {
            method: 'POST',
            body: JSON.stringify({username, password})
        })
        .then((response) => {
            if (response.ok) {
                setAuthenticated(true);
                return response.json();
            } else {
                setAuthenticated(false);
            }
        })
        .then((json) => {
            console.log(json);
            setUser(json);
        })
        .catch((e) => {
            console.error(e);
            setAuthenticated(false);
        })
    };
    return (
        <div className={styles.loginContainer}>
            <div className={styles.loginActions}>
                <input className={styles.input} type="text" placeholder="Username" />
                <input className={styles.input} type="password"  placeholder="Password" />
                <button className={styles.input} onClick={() => handleLogin()}>LOGIN</button>
            </div>
        </div>
    )
};

export default Login;