function () {

    var r = [];

    db.state.findOne( {_id: 'recCom'}, {'value':1} )
      .value
      .forEach(function(pair) {
        r.unshift({
          post: db.post.findOne({_id: pair[0]}, {title:1})
         ,user: pair[1]
        });
    });

    return r;
}
