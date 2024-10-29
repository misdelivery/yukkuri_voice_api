const express = require('express');
const routes = require('./routes/index');
const app = express();

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

app.post('/synthesize', routes.synthesize);

const PORT = process.env.PORT || 8080;
app.listen(PORT, () => {
  console.log(`Server started on port ${PORT}`);
});