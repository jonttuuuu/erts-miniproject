import {User} from '../pages/index'
import styles from '@/styles/Home.module.css'
import { useEffect, useState,useMemo } from 'react';

interface DataProps {
    user: User,
    showSessionsList: Boolean,
    setAuthenticated: Function,
}
interface SessionType {
    type: string,
    date: string,
    startTime: string,
    endTime: string,
    calories: number,
    steps: number,
    distance: number,
}
interface SessionProps {
    key: number,
    session: SessionType,
    setSelected: Function,
}

const Card = ( session: SessionProps ) => {
    return (
        <div className={styles.card} onClick={() => (session.setSelected(session))}>
            {session.session.type.charAt(0).toUpperCase() + session.session.type.slice(1)}: {session.session.date}
        </div>
    )
}

const Session = ( {selected}: SessionType | any) => {
    const name = useMemo(() => selected.session?.type.charAt(0).toUpperCase() + selected.session?.type.slice(1), [selected])
    return (
        <>
            <div className={styles.sessionContainer}>

                
                <div className={styles.sectionTitle}>
                    {name} on {selected.session?.date}
                </div>
                <div className={styles.row}>
                    <div className={styles.infoBox}>
                        Step count: <br/>
                        <div className={styles.valueContainer}>
                            <p className={styles.keyValue}>{selected.session?.steps}</p>
                        </div>
                    </div>
                    <div className={styles.infoBox}>
                        Distance walked: <br/>
                        <div className={styles.valueContainer}>
                            <p className={styles.keyValue}>{selected.session?.distance} meters</p>
                        </div>
                    </div>
                </div>
                <div className={styles.row}>
                    <div className={styles.infoBox}>
                        Average heartrate: <br/>
                        <div className={styles.valueContainer}>
                            <p className={styles.keyValue}>121</p>
                        </div>
                    </div>
                    <div className={styles.infoBox}>
                        Esimated&nbsp;calories&nbsp;burned: <br/>
                        <div className={styles.valueContainer}>
                            <p className={styles.keyValue}>{selected.session?.calories}</p>
                        </div>
                    </div>
                </div>
            </div>
        </>
    )
}
const Data = ( {user, showSessionsList, setAuthenticated} : DataProps) => {

    const [data, setData] = useState([]);
    const [selected, setSelected ] = useState(null);

    useEffect(() => {
        if (!user){
            return;
        }
        fetch(`/api/data?id=${1}`)
        .then((response) => {
            if (response.ok) {
                return response.json()
            }
        })
        .then((json) => setData(json))
        .catch((e) => console.error(e))
    },[]);

    useEffect(() => {
        console.log('list showing:',showSessionsList);
    },[showSessionsList]);

    return (
        <div className={styles.row}>
            <div className={styles.sidePanel}>
                <p className={styles.sectionTitle}>Sessions:</p>
                <div className={styles.cardContainer}>
                    {data && data.map((session,i) => (
                        <Card key={i} session={session} setSelected={setSelected}/>
                        ))}
                </div>
                <button className={styles.logout} onClick={() => setAuthenticated(false)}>Log out</button>
            </div>
            <div className={styles.info}>
                {selected && <Session selected={selected}/>}
                {!selected && 
                    <div className={styles.col} style={{textAlign:'center',justifyContent:'center', height: '100%'}}>
                        Select a workout to display its data
                    </div>
                }
            </div>
        </div>
    )
};

export default Data;