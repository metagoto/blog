function (limit) {

    var limit = limit || 10;

    var r = [];

    db.recCom.find( )
             .sort( { $natural: -1 } )
             .limit(limit)
             .forEach(function(o) {
        r.push(db.post.findOne({_id: o._id}, {title:1}));
    });
    return r;
}
