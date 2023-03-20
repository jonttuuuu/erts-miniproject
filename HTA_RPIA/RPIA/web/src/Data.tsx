import React from 'react'
import styles from './styles/Data.module.css'
import { useEffect, useState } from 'react';
import config from './config'

interface SessionType {
    id: number,
    calories: number,
    steps: number,
    distance: number,
}
interface SessionProps {
    key: number,
    session: SessionType,
    setSelected: Function,
    refresh: Function
}

const Card = ( props: SessionProps ) => {

    const handleDelete = () => {
        if (!window.confirm('Are you sure you want to permanently delete this session?')) {
            return;
        }
        fetch(`${config.backendAddr}/sessions/${props.session.id}/delete`)
        .then((response) => {
            if (!response.ok) {
                console.log('error:',response)
            }
            props.refresh((prev: number) => prev+1);
        }).catch((e) => console.log('error:',e));
    };

    return (
        <div className={styles.card} onClick={() => (props.setSelected(props.session))}>
            Hike #{props.session.id} <span style={{ cursor: 'pointer', fontSize: '25px' }}onClick={() => handleDelete()}>&times;</span>
        </div>
    )
}

const Session = ( {selected}: SessionType | any) => {
    return (
        <>
            <div className={styles.sessionContainer}>
                <div className={styles.sectionTitle}>
                    Hike #{selected.id}
                </div>
                <div className={styles.row}>
                    <div className={styles.infoBox}>
                        Step count: <br/>
                        <div className={styles.valueContainer}>
                            <p className={styles.keyValue}>{selected.steps}</p>
                        </div>
                    </div>
                    <div className={styles.infoBox}>
                        Distance walked: <br/>
                        <div className={styles.valueContainer}>
                            <p className={styles.keyValue}>{selected.distance} km</p>
                        </div>
                    </div>
                </div>
                <div className={styles.row}>
                    <div className={styles.infoBox}>
                        Esimated&nbsp;calories&nbsp;burned: <br/>
                        <div className={styles.valueContainer}>
                            <p className={styles.keyValue}>{selected.calories}</p>
                        </div>
                    </div>
                </div>
            </div>
        </>
    )
}
const Data = () => {

    const [data, setData] = useState<SessionType[]>([]);
    const [selected, setSelected ] = useState(null);
    const [error, setError] = useState(true);
    const [refresh, triggerRefresh] = useState<number>(0);

    useEffect(() => {
        fetch(`${config.backendAddr}/sessions`)
        .then((response) => {
            if (response.ok) {
                setError(false);
                return response.json();
            }else{
                setError(true);
            }
        })
        .then((json) => {
            const newData: SessionType[] = []
            json.forEach((item: Array<number>) => (
                newData.push(
                    {
                        id: item[0],
                        distance: item[1],
                        steps: item[2],
                        calories: item[3]
                    }
                )
            ))
            setData(newData)
        })
        .catch((e) => console.error(e))
    },[refresh]);

    useEffect(() => {
        setSelected(null);
    },[refresh]);

    return (
        <div className={styles.row}>
            <div className={styles.sidePanelContainer}>
                <div className={styles.sidePanel}>
                    <p className={styles.sectionTitle}>Sessions:</p>
                    <div className={styles.cardContainer}>
                        {data && data.map((session,i) => (
                            <Card key={i} session={session} setSelected={setSelected} refresh={triggerRefresh}/>
                            ))}
                    </div>
                </div>
            </div>
            <div className={styles.infoContainer}>
                <div className={styles.info}>
                    {selected && <Session selected={selected} key={refresh}/>}
                    {!selected && (!error ?
                        (<div className={styles.col} style={{textAlign:'center',justifyContent:'center', height: '100%'}}>
                            Select a workout to display its data
                        </div>
                        ):(
                            <div className={styles.col} style={{textAlign:'center',justifyContent:'center', height: '100%'}}>
                            An error occured while loading sessions
                        </div>
                        )
                        )}
                </div>
            </div>
        </div>
    )
};

export default Data;
