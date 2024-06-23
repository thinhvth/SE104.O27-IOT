import {Sequelize} from 'sequelize';
const sequelize=new Sequelize({
    host:process.env.MYSQL_HOST,
    database:process.env.MYSQL_DATABASE,
    username:process.env.MYSQL_USERNAME,
    password:process.env.MYSQL_PASSWORD,
    port:'26007',
    dialect:'mysql',
});
export default sequelize;
