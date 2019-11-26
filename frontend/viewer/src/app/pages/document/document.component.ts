import { Component, OnInit } from '@angular/core';
import { IfImporterService } from "../../services/ifimporter.service"

@Component({
  selector: 'app-document',
  templateUrl: './document.component.html',
  styleUrls: ['./document.component.css']
})
export class DocumentComponent implements OnInit {

  constructor( public ifPrvd: IfImporterService ) { }

  ngOnInit() {
    console.log("Hello")
    this.ifPrvd.getDocument("4259932")
      .subscribe(
      (res) => { console.log("Yes",res) },
      (err) => { console.error("No",err) }
    )
  }

}
