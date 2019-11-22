export class ResearchItem {

  public score: number = 0
  public docId: number = 0

  constructor( docId: number = 0, score: number = 0 ) {
    this.docId = docId
    this.score = score
  }

}

export class ResearchResults {

  public results: ResearchItem[] = []

  get resultCount() {
    return this.results.length
  }

}
