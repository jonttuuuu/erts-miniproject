import {User} from '../pages/index'

interface DataProps {
    user: User, 
}
const Data = ( {user} : DataProps) => {

    //TODO: fetch user data
    return (
        <div>
            <h2>HERE IS DATA</h2>
        </div>
    )
};

export default Data;