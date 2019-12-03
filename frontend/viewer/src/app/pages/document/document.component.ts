import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from "@angular/router";
import { IfImporterService } from "../../services/ifimporter.service"

import { Document } from "../../models/document"

@Component({
  selector: 'app-document',
  templateUrl: './document.component.html',
  styleUrls: ['./document.component.css']
})
export class DocumentComponent implements OnInit {

  public document: Document = new Document()
  public days: string[] = ["Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"]
  public months: string[] = ["January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"]

  constructor( public ifPrvd: IfImporterService, private route: ActivatedRoute ) { }

  ngOnInit() {
    let id = this.route.snapshot.paramMap.get('id') || "4259932"
    this.ifPrvd.getDocument( id )
      .subscribe(
      (res) => {
        console.log("Yes",res)
        if( !res["error"] ) {
          this.document = new Document( id, res["title"], ""+res["date"], res["contents"] )
        }
      },
      (err) => { console.error("No",err) }
    )
  }

}
