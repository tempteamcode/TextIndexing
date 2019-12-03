export class ResearchItem {

  public docId: number = 0
  public freq: number = 0
  public global_word_count: number = 0

  constructor( docId: number = 0, freq: number = 0, gwc: number = 0 ) {
    this.docId = docId
    this.freq = freq
    this.global_word_count = gwc
  }

  get score() {
    return this.freq / (this.global_word_count | 1 )
  }
}

export class ResearchResults {

  public results: ResearchItem[] = []

  get resultCount() {
    return this.results.length
  }

}
