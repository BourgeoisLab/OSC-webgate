//
// Copyright (c) 2014 Frédéric Bourgeois <bourgeoislab@gmail.com>
//
//   This file is part of OSC-webgate.
//
// http://bourgeoislab.wordpress.com/osc-webgate
// http://github.com/bourgeoislab/osc-webgate
//

//
// Global variables.
//
var polling_enabled = false;
var polling_time = 0;
var tags = undefined;
var version = "1.0"

//
// Create a tag object.
// id is a unique id for an HTML element
// variable is the variable linked to this HTML element
//
function OSCWG_tag(id, variable) {
    this.id = id;
    this.variable = variable;
    this.busy = false;
}

//
// Initialize communication.
// tags is a list of tags created with OSCWG_tag()
// polling_time is the polling time in milliseconds
//
function OSCWG_init(tags, polling_time) {
    this.tags = tags;
    this.polling_time = polling_time;
    this.polling_enabled = true;
}

//
// Set event properties of some HTML elements
//
$(document).ready(function() {
    // input text events
    $('input[type=text]').change(function() {
        writeValue($(this).attr('id'));
    });
    $('input[type=text]').focus(function() {
        setBusy($(this).attr('id'),true);
    });
    $('input[type=text]').blur(function() {
        setBusy($(this).attr('id'),false);
    });
    // input checkbox events
    $('input[type=checkbox]').change(function() {
        writeValue($(this).attr('id'));
    });
    // input range events
    $('input[type=range]').on("input change", function() {
        writeValue($(this).attr('id'));
    });
    $('input[type=range]').focus(function() {
        setBusy($(this).attr('id'),true);
    });
    $('input[type=range]').blur(function() {
        setBusy($(this).attr('id'),false);
    });
});

//
// Set or unset an HTML element as busy.
// Busy elements are not updated. 
//
function setBusy(id, busy) {
    for (i = 0; i < tags.length; i++)
    {
        if (tags[i].id === id)
            tags[i].busy = busy;
    }  
}

//
// Create a variable list from the tags.
//
function createVariableList(unique) {
    var list = [];
    for (i = 0; i < tags.length; i++)
    {
        var add = true;
        if (unique === true)
        {
            for (j = 0; j < list.length; j++)
            {
                if (tags[i].variable === list[j])
                {
                    add = false;
                    break;
                }
            }
        }
        if (add === true)
            list.push({"var" : tags[i].variable});
    }
    return list;
}

//
// Update a HTML element with a new value
//
function updateElement(tag, value)
{
    var obj;
    
    if (tag.busy === true)
        return;
        
    obj = document.getElementById(tag.id);
    if (obj === null)
        return;
        
    if ("type" in obj) {
        if (obj.type === "text") {
            obj.value = value;
        }
        else if (obj.type === "checkbox") {
            obj.checked = (value === "0") ? false : true;
        }
        else if (obj.type === "range") {
            obj.value = value;
        }
    }
    else {
        obj.innerText = value;
    }
}

//
// Self-executing function to poll variables from OSC-webgate.
//
(function poll() {
    setTimeout(function() {
        if (polling_enabled === true)
        {
            var sendData = {"version" : "1", "read" : createVariableList(false)};
            $.ajax({
                url: "/cgi-bin/json.cgi",
                type: "POST",
                dataType: "json",
                data : JSON.stringify(sendData),
                complete: poll,
                timeout: 2000,
                success: function(data) {
                    if ("read" in data) {
                        if (data.read.length === sendData.read.length) {
                            for (i = 0; i < data.read.length; i++)
                                updateElement(tags[i], data.read[i].val);
                        }
                    }
                }
            })
        }
    }, polling_time);
})();

//
// Write a value to the OSC-webgate server.
//
function writeValue(id) {
    for (i = 0; i < tags.length; i++)
    {
        if (tags[i].id === id)
        {
            var obj = document.getElementById(tags[i].id);
            if (obj === null)
                return;

            var sendData = {"version" : "1", "write" : [ {"var" : tags[i].variable, "val" : ""} ] };
            
            if ("type" in obj) {
                if (obj.type === "text") {
                    sendData.write[0].val = obj.value;
                }
                else if (obj.type === "checkbox") {
                    sendData.write[0].val = (obj.checked === false) ? "0" : "1";
                }
                else if (obj.type === "range") {
                    sendData.write[0].val = obj.value;
                }
            }
            else {
                sendData.write[0].val = obj.innerText;
            }
     
            $.ajax({
                url: "/cgi-bin/json.cgi",
                type: "POST",
                dataType: "json",
                data : JSON.stringify(sendData),
                timeout: 2000,
            })
        }
    }
}
