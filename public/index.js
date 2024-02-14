// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function() {
  // On page-load AJAX Example
  jQuery.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything 
      url: '/updateFileLogs',   //The server endpoint we are connecting to
      data: {},
      success: function (data) {
        console.log(data);
        if (data.message == "error") {
          alert("There was a problem updating the File Logs!");
        } else {
          let filesExist = 0;
          let table = "<thead><th>Image (click to download)</th><th>File name (click to download)</th><th>File size</th><th>Number of rectangles</th><th>Number of circles</th><th>Number of paths</th><th>Number of groups</th></thead><tbody>";
          let dropDown = "<option>None</option>";
        //   let allFiles = data.files;
          numFiles = data.files.length;
        for (const element of data.files) {
            filesExist = 1;
            table += '<tr class="logPanel">';
            // table data
            table += `<td><a href="../uploads/${element.name}" download><img id='logImage' src="../uploads/${element.name}" alt="Image of ${element.name}"></a></td>`; // Show image
            table += `<td><a href="../uploads/${element.name}" download>${element.name}</td>`;
            table += '<td><strong>' + element.fileSize + ' KB</strong>' + '</td>';
            table += '<td>' + element.numRect + '</td>';
            table += '<td>' + element.numCirc + '</td>';
            table += '<td>' + element.numPath + '</td>';
            table += '<td>' + element.numGroup + '</td>';
            table += '</tr>';

            dropDown += `<option>${element.name}</option>`;
        }
          if (filesExist == 0) {
              table = table + "<tr><td>No files</td><td>No files</td><td>No files</td><td>No files</td><td>No files</td><td>No files</td><td>No files</td></tr>";
          }
          table = table + "</tbody>";
          jQuery('#logPanel').html(table);
          jQuery("#svgDropDown").html(dropDown);
          console.log("File Log Panel updated with: " + JSON.stringify(data.files));
          // if (numFiles > 0 && loggedIn == 1) {
          //     $('#storeAllFiles').css('display', 'block');
          // }
      }
      },
      fail: function(error) {
          // Non-200 return, do something with error
          $('#blah').html("On page load, received error from server");
          console.log(error); 
      }
    
    // $('#svgDropDown').html(function (content) {
    //   content += '<option>Please select an image to view</option>';
    //     for (const element of data.list) {
    //       content += `<option>${element.fileName}</option>`;
    //     }

    //     return content;
    //   });
  });

  $('#svgDropDown').change(function(){
    var e = document.getElementById("svgDropDown");
    var fileName = e.options[e.selectedIndex].text;
    $('#svgTable').html("")
    // console.log("Filename3 is: "+fileName);

    if(fileName === "None"){
      $('#viewPanel').css('display', 'none');
      return;
    }

    $('#viewPanelSVG').attr("src", 'uploads/'+fileName);
    
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/updateSVG/' + fileName,

      success: function (data2) {
        $('#svgTable').append("<tr><th>Title</th><th colspan=2>Description</th></tr>");
        $('#svgTable').append("<tr><td>"+data2.file.title+"</td><td colspan=2>"+data2.file.desc+"</td></tr>");
        $('#svgTable').append("<tr><th>Component</th><th>Summary</th><th>Other attributes</th></tr>");

        let jsonParse = JSON.parse(data2.file.jsonSVG);
        // console.log("\n\nJSON PARSED: \n " + jsonParse);
        let length = jsonParse.length;
        // console.log("Length of # of objects of JSON : "+length + "\n\n");

        for(var i = 0; i < length +3; i++){
          if(i == 0) {
            if(jsonParse[i].length !== 0) {
              for (var j = 0; j < jsonParse[i].length; j++) {
                $("#svgTable").append("<tr><td>Path " + (j + 1) + "</td><td>path data = "+jsonParse[i][j]["d"]+"</td><td>" +jsonParse[i][j]["numAttr"]+ "</td></tr>");
              }
            }
          } else if(i == 1) {
            if(jsonParse[i].length !== 0) {
              for (var j = 0; j < jsonParse[i].length; j++) {
                $("#svgTable").append("<tr><td>Group " + (j + 1) + "</td><td>"+jsonParse[i][j]["children"]+" child elements</td><td>"+jsonParse[i][j]["numAttr"] +"</td></tr>");
              }
            }
          } else if(i == 2) {
            if(jsonParse[i].length !== 0) {
              for (var j = 0; j < jsonParse[i].length; j++) {
                $("#svgTable").append("<tr><td>Rectangle " + (j + 1) + "</td><td>Upper left corner: x = "+jsonParse[i][j]["x"]+  "cm, y = " +jsonParse[i][j]["y"] + "cm Width: "+jsonParse[i][j]["w"] + "cm, Height: "+jsonParse[i][j]["h"]+ "cm</td><td>" + jsonParse[i][j]["numAttr"] + "</td></tr>");
              }
            }
          } else if(i == 3) {
            if(jsonParse[i].length !== 0) {
              for (var j = 0; j < jsonParse[i].length; j++) {
                $("#svgTable").append("<tr><td>Circle " + (j + 1) + "</td><td>Centre: x = " + jsonParse[i][j]["cx"] + "cm y = " + jsonParse[i][j]["cy"] + "cm radius: " + jsonParse[i][j]["r"] + "cm <td>" + data[i][j]["numAttr"] + "</td></tr>");
              }
            }
          }
        }

      }, fail: function(error) {
        alert("Unable to load " + fileName);
        console.log(error);
      }
    });
  }); 
  
  $('#editTD').submit(function(data) {
    data.preventDefault();

    let title = $("#edittitle").val();
    let description = $("#editdesc").val();
    // var e = document.getElementById("svgDropDown");
    // var file = e.options[e.selectedIndex].innerHTML;
    let file = $("#svgDropDown option:selected").text();

    // console.log("Filename4 is: "+file);

    $.ajax({
      type: 'get',
      dataType: 'text',
      url: '/editTD/',
      data: {
        file: file,
        title: title,
        description: description
      },
      success: function (data) {
        console.log(data);
        console.log("in success");
        alert("Successfully Added");
        /*Reloads the page to display latest data */
        location.reload();
      },
      fail: function(error) {

        alert("Could not add component to " + filename);
        console.log(error);
      }
    });

  });

  /* Create New SVG Image */
  $('#createSVG').submit(function(data) {
    // Do not allow submitting the form empty
    data.preventDefault();
    let file = $("#fileName").val() +".svg";
    console.log("User entered SVG image Name is: " + file);

    let title = $("#title").val();
    let description = $("#description").val();

    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/createSVGimg/',
      data: {
        fileName: file,
        title: title,
        description: description
      },
      success: function(json) {

        // if ((json["numRect"] === 0) && (json["numCirc"]=== 0) && (json["numPaths"]=== 0) && (json["numGroups"]=== 0))
        // {
        //   console.log("File " + json["filename"] + " has no shapes. Will NOT add to Log Panel (SKIPPED)");
        // }else{
        //   $('#table').append("<tr><td><img id='logImage' src='" + file + "'></td><td><a href='" + file + "'>" + file +"</a></td><td>" + json["filesize"] + "</td><td>" + json["numRect"] + "</td><td>"+ json["numCirc"] + "</td><td>" + json["numPaths"] + "</td><td>" + json["numGroups"] + "</td></tr>");
        // }
        // jQuery("#svgDropDown").html(dropDown);
        $('#svgDropDown').append("<option value ='" + fileName + "'>" + fileName + "</option>");
        // $('#dropdown2').append("<option value ='" + file + "'>" + file + "</option>");
        // $('#dropdown3').append("<option value ='" + file + "'>" + file + "</option>");

        alert("Successfully added file: " + fileName);
        location.reload();
        console.log(data);
      },
      fail: function(error) {
        alert("Unable to create file: " + file);
        console.log(error);
      }
    });
  });

  // Event listener form example , we can use this instead explicitly listening for events
  // No redirects if possible
  // $('#someform').submit(function(e){
  //     $('#blah').html("Form has data: "+$('#entryBox').val());
  //     e.preventDefault();
  //     //Pass data to the Ajax call, so it gets passed to the server
  //     $.ajax({
  //         //Create an object for connecting to another waypoint
  //     });
  // });
});