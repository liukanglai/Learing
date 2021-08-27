# Hugo

## install

- sudo pacman -S hugo
- hugo version
- hugo new site studyhugo

## quickstart

1. git init

2. config.toml
    - git submodule add `https://github.com/olOwOlo/hugo-theme-even themes/even`
	- cp themes/even/config.toml ./

3. content
	- hugo new posts/my-first-post.md (in even, need post)

4. hugo serve -D(-D make draft push)

5. `http://127.0.0.1:1313`

> build static pages: hugo -D, int /public, (don't need if you use github pages)

## github pages

```
echo "# liukanglai.github.io" >> README.md
git add .
git commit -m "first commit"
git branch -M main
git remote add origin git@github.com:liukanglai/liukanglai.github.io.git
git push -u origin main
```

- git actions -> set up a workflow yourself -> .github/workflows/gh-pages.yml
- paste the [code](https://gohugo.io/hosting-and-deployment/hosting-on-github/)
- commit

- git pull

- 需要修改config.toml里的baseURL的参数，确保改成username.github.io
- baseURL = "http://nightan42643.github.io"

> the draft will not publish

- domain:
- static/CNAME: write your domain: liukanglai.tk

## config
