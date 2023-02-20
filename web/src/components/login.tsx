import {User} from '../pages/index';
import {useState} from 'react';
import styles from '@/styles/Home.module.css'

interface LoginProps {
    setAuthenticated: React.Dispatch<React.SetStateAction<boolean>>,
    setUser: React.Dispatch<React.SetStateAction<User>>
}

const Login = (
    {setAuthenticated, setUser}: LoginProps) => {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');

    const handleLogin = () => {
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
        <div className={styles.col}>
            <h2 style={{ textAlign: 'center'}}>LOGIN</h2>
            <div className={styles.row}>
                <div className={styles.col}>
                    <p>
                        Username:
                    </p>
                    <p>
                    Password:
                    </p>
                </div>
                <div className={styles.col}>
                    <input type='text' value={username} onChange={(v) => setUsername(v.target.value)}/>
                    <input
                        type='password'
                        value={password}
                        onChange={(v) => setPassword(v.target.value)}
                        onKeyDown={(event) => event.key === 'Enter' && handleLogin()}
                    />
                </div>
                    <button onClick={() => handleLogin()}>LOGIN</button>
                </div>
        </div>
    )
};

export default Login;