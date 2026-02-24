const canvas = document.getElementById("gameCanvas");
const ctx = canvas.getContext("2d");

const scoreElement = document.getElementById("score");
const highScoreElement = document.getElementById("high-score");

const startBtn = document.getElementById("start-btn");
const restartBtn = document.getElementById("restart-btn");

const pauseBtn = document.getElementById("pause-btn");
const resumeBtn = document.getElementById("resume-btn");

const overlay = document.getElementById("game-overlay");
const startOverlay = document.getElementById("start-overlay");

const finalScoreElement = document.getElementById("final-score");

const boxSize = 20;

let snake;
let food;

let score;
let highScore = 0;

let moveX;
let moveY;

let gameInterval;

let gameSpeed = 200;
let speedIncrease = 5;
let minSpeed = 60;

let isPaused = false;
let isGameRunning = false;



function startGame() {
    snake = [
        { x: 200, y: 200 }
    ];

    food = generateFood();

    score = 0;

    moveX = boxSize;
    moveY = 0;

    gameSpeed = 200;

    isPaused = false;
    isGameRunning = true;

    scoreElement.textContent = score;

    overlay.classList.add("hidden");
    startOverlay.classList.add("hidden");

    pauseBtn.style.display = "inline-block";
    resumeBtn.style.display = "none";

    clearInterval(gameInterval);

    gameInterval = setInterval(gameLoop, gameSpeed);
}



function generateFood() {
    let foodX = Math.floor(Math.random() * canvas.width / boxSize) * boxSize;
    let foodY = Math.floor(Math.random() * canvas.height / boxSize) * boxSize;

    return { x: foodX, y: foodY };
}



function gameLoop() {
    if (isPaused) return;

    moveSnake();

    if (checkCollision()) {
        endGame();
        return;
    }

    drawGame();
}



function moveSnake() {
    const newHead = {
        x: snake[0].x + moveX,
        y: snake[0].y + moveY
    };

    snake.unshift(newHead);

    if (newHead.x === food.x && newHead.y === food.y) {
        score++;

        scoreElement.textContent = score;

        food = generateFood();

        increaseSpeed();
    }
    else {
        snake.pop();
    }
}



function increaseSpeed() {
    if (gameSpeed > minSpeed) {
        gameSpeed -= speedIncrease;

        clearInterval(gameInterval);

        gameInterval = setInterval(gameLoop, gameSpeed);
    }
}



function checkCollision() {
    const head = snake[0];

    if (head.x < 0 || head.y < 0 ||
        head.x >= canvas.width ||
        head.y >= canvas.height)
        return true;

    for (let i = 1; i < snake.length; i++) {
        if (head.x === snake[i].x && head.y === snake[i].y)
            return true;
    }

    return false;
}



function drawGame() {
    ctx.clearRect(0, 0, canvas.width, canvas.height); // Let gradient show through

    // Draw grid effect
    ctx.strokeStyle = "rgba(255,255,255,0.05)";
    for (let i = 0; i < canvas.width; i += boxSize) {
        ctx.beginPath(); ctx.moveTo(i, 0); ctx.lineTo(i, canvas.height); ctx.stroke();
    }
    for (let i = 0; i < canvas.height; i += boxSize) {
        ctx.beginPath(); ctx.moveTo(0, i); ctx.lineTo(canvas.width, i); ctx.stroke();
    }

    // Shadow context for 3D effect on items
    ctx.shadowBlur = 10;

    // Draw Food
    ctx.shadowColor = "#ff3b30";
    ctx.fillStyle = "#ff3b30";
    ctx.beginPath();
    ctx.arc(food.x + boxSize / 2, food.y + boxSize / 2, boxSize / 2 - 2, 0, Math.PI * 2);
    ctx.fill();

    // Draw Snake
    ctx.shadowColor = "rgba(255, 255, 255, 0.4)";
    ctx.fillStyle = "#ffffff";

    snake.forEach((part, index) => {
        // Head is solid white, body slightly transparent
        ctx.globalAlpha = index === 0 ? 1 : 0.8;
        ctx.fillRect(part.x + 1, part.y + 1, boxSize - 2, boxSize - 2);
    });

    // reset effects
    ctx.globalAlpha = 1;
    ctx.shadowBlur = 0;
}



function endGame() {
    isGameRunning = false;

    clearInterval(gameInterval);

    if (score > highScore) {
        highScore = score;
        highScoreElement.textContent = highScore;
    }

    finalScoreElement.textContent = score;

    overlay.classList.remove("hidden");
}



document.addEventListener("keydown", changeDirection);



function changeDirection(event) {
    const key = event.key;

    if (key === "ArrowLeft" || key === "a") {
        if (moveX === 0) {
            moveX = -boxSize;
            moveY = 0;
        }
    }

    if (key === "ArrowRight" || key === "d") {
        if (moveX === 0) {
            moveX = boxSize;
            moveY = 0;
        }
    }

    if (key === "ArrowUp" || key === "w") {
        if (moveY === 0) {
            moveX = 0;
            moveY = -boxSize;
        }
    }

    if (key === "ArrowDown" || key === "s") {
        if (moveY === 0) {
            moveX = 0;
            moveY = boxSize;
        }
    }
}



pauseBtn.onclick = function () {
    if (isGameRunning) {
        isPaused = true;
        pauseBtn.style.display = "none";
        resumeBtn.style.display = "inline-block";
    }
}



resumeBtn.onclick = function () {
    if (isGameRunning) {
        isPaused = false;
        pauseBtn.style.display = "inline-block";
        resumeBtn.style.display = "none";
    }
}



startBtn.onclick = startGame;

restartBtn.onclick = startGame;