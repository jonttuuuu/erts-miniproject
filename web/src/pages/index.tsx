import Head from 'next/head'
import Image from 'next/image'
import { Inter } from '@next/font/google'
import styles from '@/styles/Home.module.css'
import type {NextPage} from 'next';
import {useMemo, useState} from 'react';
import Login from '../components/login';
import Data from '../components/data';

const inter = Inter({ subsets: ['latin'] })

export interface User {
  id: string;
  token: string;
}

const Home: NextPage = () => {
  const [authenticated, setAuthenticated] = useState(false);
  const [user, setUser] = useState<User>({id: '', token: ''});
  return (
    <>
      <Head>
        <title>Hiking data</title>
        <meta name="description" content="Hiking data visualization" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <link rel="icon" href="/favicon.ico" />
      </Head>
        <div className={styles.root}>
          <div className={styles.header}>
            {authenticated && (
              <button onClick={() => setAuthenticated(false)}>LOGOUT</button>
            )}

          </div>
          <main className={styles.main}>
            {authenticated ? <Data user={user}/> : <Login setAuthenticated={setAuthenticated} setUser={setUser}/>}
          </main>

        </div>

    </>
  )
}

export default Home;
