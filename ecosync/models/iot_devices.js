import {DataTypes} from 'sequelize';
import sequelize from '../configs/database.js';
const IotDevices=sequelize.define('iot_devices',{
    id:{
        type:DataTypes.UUIDV4,
        allowNull:false, 
        primaryKey:true,  
    },
    ip:{
        type:DataTypes.STRING(100),
        allowNull:true,
    },
    home_id:{
        type:DataTypes.CHAR(36),
        allowNull:true,
    },
    air_val:{
        type:DataTypes.DOUBLE,
        allowNull:true,
    },
    left_status:{
        type:DataTypes.DOUBLE,
        allowNull:true,
    },
    right_status:{
        type:DataTypes.DOUBLE,
        allowNull:true,
    },
    status:{
        type:DataTypes.STRING(100),
        allowNull:true,
    },
    created_at:{
        type:DataTypes.DATE,
        allowNull:true,
    },
    updated_at:{
        type:DataTypes.DATE,
        allowNull:true,
    }
},{
    timestamps:false,
    tableName:'iot_devices'
})
export default IotDevices;