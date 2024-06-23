import {Sequelize} from 'sequelize';
const sequelize=new Sequelize({
    host:'ecosync-ecosync.f.aivencloud.com',
    database:'ecosync',
    username:'avnadmin',
    password:'AVNS_A7a4SSPysBOacl6YCmf',
    port:'26007',
    dialect:'mysql',
});
export default sequelize;
