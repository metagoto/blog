function (id, user, limit) {

    var limit = limit || 10;

    var o = db.state.findOne( {_id: 'recCom'} );

    if (!o || !o.value)
        return;

    if (o.value.length >= limit)
        db.state.update( {_id: 'recCom'}, { '$pop': { 'value': -1 } } );

    db.state.update( {_id: 'recCom'}, { '$push': { 'value': [id, user] } } );
}
