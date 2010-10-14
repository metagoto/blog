function () {

    var r = [];
    var buf = {};

    var value = db.state.findOne( {_id: 'recCom'}, {'value':1} ).value;

    value.reverse().forEach(function(pair) {

        var id = pair[0];
        buf[id] = buf[id] || db.post.findOne({_id: id}, {title:1, coms:1});

        var com = buf[id].coms.pop();

        r.push({
          post: {_id: id, title: buf[id].title },
          user: com.user,
          date: com.date,
          content: com.content
        });

    });

    return r;
}
