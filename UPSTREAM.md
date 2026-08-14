# Upstream provenance

The authors of the LPR paper do not publish this code through a Git repository. The
official project page links directly to a ZIP archive containing one source file:

- Project page: <https://leria-info.univ-angers.fr/~jinkao.hao/bcp.html>
- Archive: <https://leria-info.univ-angers.fr/~jinkao.hao/bcp/SourceCodeLPR.zip>
- Archive `Last-Modified` header: `Fri, 10 Mar 2017 04:12:15 GMT`
- Archive SHA-256: `0326a2bdde399cae6fef65a4528638ce9b928ccccfdde916c1c5ed580f6aa981`
- Extracted `LPR_BCP.cpp` SHA-256: `67693af81f3e7344c579ad14af75a90f9eb8ccfbc2e84bb0035894cb20ba353b`

Commit `782aef1` and tag `upstream-2017-03-10` preserve the downloaded archive and
the extracted source byte-for-byte. Development takes place on the `review-rerun`
branch, so all changes can be inspected with:

```sh
git diff upstream-2017-03-10..review-rerun
```

The archive contains no license file or license notice. Availability for download does
not by itself grant redistribution rights. Permission from the authors should be obtained
before publishing a modified copy outside the research workspace.
