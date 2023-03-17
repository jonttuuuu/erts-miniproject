import React, {useState} from 'react';
import styles from './styles/Home.module.css'
import './App.css';
import Data from './Data'
import Login from './Login'

export interface User {
  id: string;
  token: string;
}

const App = () => {
  const [authenticated, setAuthenticated] = useState(true);
  const [user, setUser] = useState<User>({id: '', token: ''});
  return (
    <div className={styles.root}>
    <main className={styles.main}>
      {authenticated ? <Data user={user} setAuthenticated={setAuthenticated}/> : <Login setAuthenticated={setAuthenticated} setUser={setUser}/>}
    </main>

  </div>
  );
}

export default App;
