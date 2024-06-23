import mysql from 'mysql2'
import mqtt from 'mqtt'
import fs from 'fs'
import {format} from 'date-fns'
import dotenv from 'dotenv';
import IotDevices from './models/iot_devices.js'
import sequelize from './configs/database.js'
import { v4 as uuidv4 } from 'uuid';
function getcurrenttime(){
    return format(new Date(),'yyyy-MM-dd HH:mm:ss')
}
var connection=mysql.createConnection({
    host: process.env.MYSQL_HOST,
    user: process.env.MYSQL_USERNAME,
    password: process.env.MYSQL_PASSWORD,
    port: 26007,
    database: 'ecosync',
})
connection.connect((err)=>{
    if(err){
        console.log(err)
    }else{
        console.log("Connected to database")
    }
})
const sensorTopic="client/+/sensor"
const CA_CERT=fs.readFileSync('./emqx-ca.pem')
const host = 'wss://oe9219e1.ala.eu-central-1.emqxsl.com:8084/mqtt'
const options={
    clientId: "web-server",
    username: process.env.MQTT_USERNAME,
    password: process.env.MQTT_PASSWORD,
    rejectUnauthorized:true,
    ca:CA_CERT,
}
const client=mqtt.connect(host,options)

client.on('connect',()=>{
    console.log("connected to mqtt")
    client.subscribe(sensorTopic)
})

var device_data;
client.on('message',(topic,message)=>{
    device_data=JSON.parse(message.toString())
    sequelize.authenticate();
    IotDevices.sync();
    const newUser = IotDevices.create({
        id:uuidv4(),
        ip:device_data.ip,
        air_val:device_data.air_val,
        left_status:device_data.left_status,
        right_status:device_data.right_status,
        status:device_data.status,
        created_at:getcurrenttime(),
        updated_at:getcurrenttime()
    })
})

