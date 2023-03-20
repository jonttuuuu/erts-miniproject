import React from 'react';
import styles from './styles/App.module.css'
import Data from './Data'

export interface User {
    id: string;
    token: string;
}

const App = () => {
    return (
        <div className={styles.root}>
            <main className={styles.main}>
                <Data/>
            </main>
        </div>
    );
}

export default App;
